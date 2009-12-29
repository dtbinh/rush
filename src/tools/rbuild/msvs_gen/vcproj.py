import uuid

class VCProjType:
    Exe = 0,
    Lib = 1,
    Dll = 2


class VCProjConf:
    """.vcproj configuration"""
    def __init__(self):
        self.build = "Debug"
        self.includeDirs = []
        self.libs = []
        self.proj = None
        self.files = []
        self.postBuildCmd = ""
        
    def getConfigName(self):
        if self.build == "Debug":
            return "Debug|Win32"
        else:
            return "Release|Win32"
        
    def getOutDir(self):
        if self.build == "Debug":
            return "_debug/" + self.proj.name
        else:
            return "_release/" + self.proj.name
        
    def getTypeIdx(self):
        if self.proj.type == VCProjType.Exe: return "1"
        if self.proj.type == VCProjType.Lib: return "4"
        if self.proj.type == VCProjType.Dll: return "2"
        return "0"
        
    def getIncludeDirsStr(self):
        res = ""
        for incdir in self.includeDirs:
            res += incdir + ";"
        return res
    
    def getSuffix(self):
        if self.build == "Debug":
            return "_d"
        else:
            if self.proj.type == VCProjType.Exe:
                return ""
            else:
                return "_r"
    
   
class VCProj:
    """.vcproj file"""
    def __init__(self):
        self.name = ""
        self.configs = []
        self.depends = []
        self.srcDir = ""
        self.type = VCProjType.Lib
        self.guid = ('{' + str(uuid.uuid1()) + '}').upper()
        
class VCSolution:
    """.sln file"""
    def __init__(self):
        self.name = ""
        self.projects = []
        
    def getProjByName(self, name):
        for proj in self.projects:
            if proj.name == name: return proj
        return None
