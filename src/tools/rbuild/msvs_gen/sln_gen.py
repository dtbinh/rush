import os
import glob
import copy

import vcproj_gen
import depends
from vcproj import VCProjConf, VCProjType, VCProj, VCSolution
    
def dirFiles(dir, mask):
    return map(lambda x: dir + os.path.split(x)[1], glob.glob(mask))

def genTestProj(proj):
    testProj = VCProj()
    testProj.type = VCProjType.Exe
    testProj.compiler = proj.compiler
    testProj.name = "test_" + proj.name.replace("rb_", "") 
    testProj.srcDir = os.path.join( proj.src_dir, "test" )
    if not os.path.exists(testProj.srcDir):
        return None
    
    ref_dir = os.path.join( '..\\..\\src\\engine\\', proj.name )
    ref_dir = os.path.join(ref_dir, 'test\\' )
    files = dirFiles(ref_dir, testProj.srcDir + '/*.*')
    if len(files) == 0:
    	return None
    
    for conf in proj.configs:
        testConf = VCProjConf()
        testConf.files = files
        testConf.build = conf.build
        testConf.proj = testProj
        testProj.configs.append( testConf )
        postBuildCmd = '..\\..\\bin\\%s%s.exe --media="$(InputDir)\..\..\src\engine\%s\test\media\"' % (testProj.name, testConf.getSuffix(), proj.name)
        testConf.postBuildCmd = postBuildCmd
        
            
    testProj.depends = proj.depends[:] + [proj.name]
    testProj.depends += ["unittest++"] 
    testProj.depends += ["tinyxml"]

    return testProj
    
def genProj(name, type, env):
    proj = VCProj()
    proj.name = name
    proj.type = type
    proj.src_dir = env['root_dir'] + 'src/engine/' + name + '/'
    proj.compiler = env['compiler']
    
    # extract dependencies
    depends_text = ""
    deps = {}
    try:
       fp = open(os.path.join(proj.src_dir, 'depends.txt'), 'rb')
       depends_text = fp.read()
       fp.close()
       deps = depends.DepParser().parse(depends_text)
    except:
        pass
    int_libs = []
    if 'int_libs' in deps: int_libs = deps['int_libs']
    ext_libs = []
    if 'ext_libs' in deps: ext_libs = deps['ext_libs']
    
    ref_dir = '..\\..\\src\\engine\\' + name + '\\'
    file_list = dirFiles(ref_dir, proj.src_dir + '/*.*')
    
    proj_d = VCProjConf()
    proj_d.files = file_list
    proj_d.build = "Debug"
    proj_d.proj = proj
    
    proj_r = copy.copy(proj_d)
    proj_r.build = "Release"
    
    proj.configs = [proj_d, proj_r]
    proj.depends = ext_libs + int_libs
    return proj
    
def genDependPaths(proj, sln, env):
    int_libs = env['int_libs']
    ext_libs = env['ext_libs']
    for conf in proj.configs:
        libSuffix = "_d"
        if conf.build == "Release":
        	libSuffix = "_r"
        conf.includeDirs = ['..\\..\\src\\include']
        conf.libs = env['sys_libs'][:]
        for dep in proj.depends:
            # includes from internal libraries
            if dep in int_libs:
                conf.includeDirs += [os.path.join( '..\\..\\src\\engine\\', dep )]
            if dep in ext_libs:
                conf.includeDirs += [os.path.join( '..\\..\\external\\', ext_libs[dep] )]
            #  additional libraries to link with
            if proj.type != VCProjType.Lib:
                if dep in ext_libs:
                    conf.libs += [dep + libSuffix + ".lib"]
                depProj = sln.getProjByName(dep)
                if depProj != None and depProj.type == VCProjType.Lib:
            	   conf.libs += [dep + libSuffix + ".lib"]

slnProjTemplate = """\
Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"%(name)s\", \"%(name)s.vcproj\", \"%(guid)s\"
ProjectSection(ProjectDependencies) = postProject
%(depList)sEndProjectSection

"""

slnGlobalTemplate = """\
Global
    GlobalSection(SolutionConfiguration) = preSolution
        Debug = Debug
        Release = Release
    EndGlobalSection
    GlobalSection(ProjectDependencies) = postSolution
    EndGlobalSection
    GlobalSection(ProjectConfiguration) = postSolution
        %(configs)s
    EndGlobalSection
    GlobalSection(ExtensibilityGlobals) = postSolution
    EndGlobalSection
    GlobalSection(ExtensibilityAddIns) = postSolution
    EndGlobalSection
EndGlobal
"""

def createSlnScript(env, sln):
    if env['compiler'] == 'vc9':
        script = "Microsoft Visual Studio Solution File, Format Version 10.00\n" +\
        "# Visual C++ Express 2008\n"
    else:
        script = "Microsoft Visual Studio Solution File, Format Version 8.00\n"
    configStr = ""
        
    for proj in sln.projects:
    	depList = ""
    	for dep in proj.depends:
    		depProj = sln.getProjByName(dep)
    		if depProj != None and proj.type != VCProjType.Lib:
    			depList += "\t\t%(guid)s = %(guid)s\n" % {'guid':depProj.guid}
        script += slnProjTemplate %{'name':proj.name, 'depList':depList, 'guid':proj.guid}
        for conf in proj.configs:
            configStr += ("\t\t%(guid)s.%(build)s.ActiveCfg = %(cfgname)s\n" +\
                          "\t\t%(guid)s.%(build)s.Build.0 = %(cfgname)s\n") %\
                         {"guid":proj.guid, 
                          "build":conf.build, 
                          "cfgname":conf.getConfigName()}
    script += slnGlobalTemplate %{'configs':configStr}
    return script

def genSolution(env, name):     
    int_libs = env['int_libs']   
    sln = VCSolution()
    sln.name = name
    for static_lib in int_libs:
        proj = genProj(static_lib, VCProjType.Lib, env)
        sln.projects.append( proj )
        testProj = genTestProj(proj)
        if testProj != None:
            sln.projects.append( testProj )
       
    sln.projects.append( genProj("rboot", VCProjType.Exe, env) )
    
    build_root = env['root_dir'] + 'build/' + env['compiler'] + '/'
    
    for proj in sln.projects:
        genDependPaths(proj, sln, env)
        proj.script = vcproj_gen.create_vcproj(proj)
        proj.outPath = build_root + proj.name + '.vcproj'
        
    for proj in sln.projects:
        f = open(proj.outPath, "wt")
        f.writelines(proj.script)
        f.close()
        
    #  write solution file
    sln.outPath = build_root + sln.name + '.sln'
    sln.script = createSlnScript(env, sln)
    
    f = open(sln.outPath, "wt")
    f.writelines(sln.script)
    f.close()
