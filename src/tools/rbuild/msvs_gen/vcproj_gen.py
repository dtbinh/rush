import os.path
import xmlgen
from vcproj import VCProj, VCProjConf, VCProjType

def config_compiler(comp_node, config):
    comp_node.add_attr("AdditionalIncludeDirectories", config.getIncludeDirsStr())
    comp_node.add_attr("RuntimeTypeInfo", "FALSE")
    comp_node.add_attr("ExceptionHandling", "FALSE")
    if config.proj.compiler == 'vc9':
        pch_tag = "2"
    else:
        pch_tag = '3'
    comp_node.add_attr("UsePrecompiledHeader", pch_tag)
    comp_node.add_attr("PrecompiledHeaderThrough", "stdafx.h")
    comp_node.add_attr("ProgramDataBaseFileName", "$(IntDir)//rush.pdb")
    comp_node.add_attr("ShowIncludes", "FALSE")
    comp_node.add_attr("WarningLevel", "3")
    
    if config.build == "Debug":
        comp_node.add_attr("Optimization", "0")
        comp_node.add_attr("PreprocessorDefinitions", "WIN32;_DEBUG;_LIB")
        comp_node.add_attr("MinimalRebuild", "TRUE")
        comp_node.add_attr("BasicRuntimeChecks", "3")
        comp_node.add_attr("RuntimeLibrary", "1")
        comp_node.add_attr("DebugInformationFormat", "4")
    else:
        comp_node.add_attr("Optimization", "3")
        comp_node.add_attr("PreprocessorDefinitions", "WIN32;NDEBUG;_LIB")
        comp_node.add_attr("BasicRuntimeChecks", "0")
        comp_node.add_attr("RuntimeLibrary", "0")
        comp_node.add_attr("DebugInformationFormat", "3")
        comp_node.add_attr("GlobalOptimizations", "TRUE")
        comp_node.add_attr("InlineFunctionExpansion", "2")
        comp_node.add_attr("FavorSizeOrSpeed", "1")
        comp_node.add_attr("OmitFramePointers", "TRUE")
        comp_node.add_attr("OptimizeForWindowsApplication", "TRUE")
        comp_node.add_attr("StringPooling", "TRUE")
        comp_node.add_attr("StructMemberAlignment", "0")
        comp_node.add_attr("BufferSecurityCheck", "FALSE")
        comp_node.add_attr("EnableFunctionLevelLinking", "TRUE")
        comp_node.add_attr("CallingConvention", "0")

def create_filter(parent, name, filter, file_list):
    if len(file_list) == 0:
        return
    filter_node = parent.add_child("Filter")
    filter_node.add_attr("Name", name)    
    filter_node.add_attr("Input", filter) 
    for file_path in  file_list:
        file_node = filter_node.add_child("File")
        file_node.add_attr("RelativePath", file_path)
        if os.path.split( file_path )[1] == "stdafx.cpp":
            d_conf = file_node.add_child( "FileConfiguration")
            d_conf.add_attr("Name", "Debug|Win32")
            d_tool = d_conf.add_child( "Tool")
            d_tool.add_attr("Name", "VCCLCompilerTool")
            d_tool.add_attr("UsePrecompiledHeader", "1")
            
            r_conf = file_node.add_child("FileConfiguration")
            r_conf.add_attr("Name", "Release|Win32")
            r_tool = r_conf.add_child("Tool")
            r_tool.add_attr("Name", "VCCLCompilerTool")
            r_tool.add_attr("UsePrecompiledHeader", "1")
            
    return filter_node

def create_tool(config_node, name, attrib_list = []):
    tool_node = config_node.add_child("Tool")
    tool_node.add_attr("Name", name)    
    return tool_node

def create_config(parent, config):
    config_node = parent.add_child("Configuration")
    config_node.add_attr("Name", config.getConfigName())
    config_node.add_attr("OutputDirectory", config.getOutDir())
    config_node.add_attr("IntermediateDirectory", config.getOutDir())
    config_node.add_attr("ConfigurationType", config.getTypeIdx()) 
    config_node.add_attr("CharacterSet", "2")
    config_node.add_attr("UseOfMFC", "0")
    config_node.add_attr("ATLMinimizesCRunTimeLibraryUsage", "FALSE")
    
    type = config.proj.type
    if type == VCProjType.Lib:
        librarian_node = create_tool(config_node, "VCLibrarianTool")
        out_path = "..\\..\\lib\\" + config.proj.compiler + "\\" + config.proj.name
        if config.build == "Debug":
            out_path += "_d.lib"
        else:
            out_path += "_r.lib"
        librarian_node.add_attr("OutputFile", out_path )
    elif type == VCProjType.Exe:
        linker_node = create_tool(config_node, "VCLinkerTool")
        linker_node.add_attr("AdditionalDependencies", " ".join( config.libs ) )
        linker_node.add_attr("AdditionalLibraryDirectories", '..\\..\\lib\\' + config.proj.compiler)
        linker_node.add_attr("IgnoreDefaultLibraryNames", "libci.lib;libcd.lib;")
        linker_node.add_attr("GenerateDebugInformation", "TRUE")
        linker_node.add_attr("SubSystem", "2")
        linker_node.add_attr("TargetMachine", "1")
                
        out_path = "..\\..\\bin\\" + config.proj.name
        if config.build == "Debug":
            out_path += "_d.exe"
            linker_node.add_attr("LinkIncremental", "2")
        else:
            out_path += ".exe"  
            linker_node.add_attr("LinkIncremental", "1")
            linker_node.add_attr("OptimizeReferences", "2")
            linker_node.add_attr("EnableCOMDATFolding", "2")
        linker_node.add_attr("OutputFile", out_path)
        
    # post-build event tool
    tool_node = config_node.add_child("Tool")
    tool_node.add_attr("Name", "VCPostBuildEventTool")
    tool_node.add_attr("CommandLine", config.postBuildCmd)

    
    compiler_node = create_tool(config_node, "VCCLCompilerTool")
    config_compiler(compiler_node, config)
    
    create_tool(config_node, "VCMIDLTool")
    create_tool(config_node, "VCPreBuildEventTool")
    create_tool(config_node, "VCPreLinkEventTool")
    create_tool(config_node, "VCWebServiceProxyGeneratorTool")
    create_tool(config_node, "VCXMLDataGeneratorTool")
    create_tool(config_node, "VCManagedWrapperGeneratorTool")
    create_tool(config_node, "VCAuxiliaryManagedWrapperGeneratorTool")
    create_tool(config_node, "VCResourceCompilerTool")
    return config_node
        
def create_vcproj(proj):
    #  <VisualStudioProject>
    proj_node = xmlgen.Node("VisualStudioProject")
    proj_node.add_attr("ProjectType", "Visual C++")
    
    if proj.compiler == 'vc9':
        proj_node.add_attr("Version", "9,00")
        proj_node.add_attr("TargetFrameworkVersion", "131072")
    else:
        proj_node.add_attr("Version", "7.10")
        
    proj_node.add_attr("Name", proj.name)
    proj_node.add_attr("Keyword", "Win32Proj")
    proj_node.add_attr("ProjectGUID", proj.guid)

    #  <Platforms>
    platforms_node = proj_node.add_child("Platforms")

    win32_platform_node = platforms_node.add_child("Platform") 
    win32_platform_node.add_attr("Name", "Win32")

    #  <Configurations>
    configs_node = proj_node.add_child("Configurations")
    
    for conf in proj.configs:
        create_config(configs_node, conf) 
    
    #  <References>
    refs_node = proj_node.add_child("References")

    #  <Files>
    files_node = proj_node.add_child("Files")
    

    def filter_files(files, filter):
        filter_list = filter.split(";")
        def ext_in_filter(f):
            ext = os.path.splitext(f)[1]
            ext = ext.strip(".")
            return ext in filter_list
        return [f for f in files if ext_in_filter( f ) ]
    
    file_list = proj.configs[0].files 
    src_filter = "cpp;h"
    create_filter(files_node, "src", src_filter, filter_files(file_list, src_filter))
    res_filter = "rc;ico;cur;bmp;rc2;rct;jpg;jpeg;resx;txt"
    create_filter(files_node, "res", res_filter, filter_files(file_list, res_filter))
    
    #  <Globals>
    globals_node = proj_node.add_child("Globals") 
    return proj_node.to_string_doc(encoding = "Windows-1252")


