import os, inspect
import sys

#windows platform specific import
if sys.platform == 'win32':
        import _winreg as winreg

#Pass Installation registry KEY NAME along with VALUE NAME
def getRegValue(keyName, valueName):
	reg = winreg.ConnectRegistry(None, winreg.HKEY_LOCAL_MACHINE)
	key = winreg.OpenKey(reg, keyName) 
	value = winreg.QueryValueEx(key, valueName)
	winreg.CloseKey(key)
	return value[0]
	
# uVision4 Keil_ARM_MDK armcc compiler's system include
def sysInclude():
	try:
		current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
		scriptFile = inspect.getfile(inspect.currentframe())
		fileName = os.path.basename(scriptFile)
		cipFilename = os.path.splitext(fileName.strip())[0] + '.cip'
		normal_parent = os.path.dirname(current_dir)
		dataDir = os.path.dirname(normal_parent)
		cctDir = os.path.dirname(dataDir)
		configDir = os.path.dirname(cctDir)
		cipDir = os.path.join(configDir, "cip")
		cipFilepath = os.path.join(cipDir, cipFilename)
		stubDir = os.path.join(normal_parent, "Stub")
		cipFile = open(cipFilepath, 'w')
		
		# Keil uVision4 installation path
		# Checking if registry entry is available
		exists = True
		try:
			KeilInstall = getRegValue(r'SOFTWARE\Wow6432Node\keil\Products\MDK','Path')
		except WindowsError:
			exists = False		
		if not exists:
			try:
				KeilInstall = getRegValue(r'SOFTWARE\keil\Products\MDK','Path')
			except WindowsError:
				KeilInstall = None
		#if compiler Installation not found then create cip file with Error message	
		if KeilInstall is None:
			cipFile.write("* Error: Keil uVision4 for MDK - ARMCC Compiler installation registry is not valid \n* please report at www.perforce.com/support/request-support")
			return KeilInstall
			
		# stub directory  
		stubList = [os.path.join(d, t) for d, ds, fs in os.walk(stubDir) for t in ds]
		for dirName in stubList:
			cipFile.write('-i "' + dirName + '"\n')
			cipFile.write('-q "' + dirName + '"\n')
		for fiDir in stubList:
			if fiDir.endswith("prlforceinclude"):
				fileList = [os.path.join(fiDir, x) for x in os.listdir(fiDir)]
				fileList.sort()
				for fn in fileList:
					cipFile.write('-fi "' + fn + '"\n')

		#armcc compiler include 
		armccInclude = os.path.join(KeilInstall, "ARMCC", "include")
		cipFile.write('-i "' + armccInclude + '"\n')
		cipFile.write('-q "' + armccInclude + '"\n')
		incList = [os.path.join(d, t) for d, ds, fs in os.walk(armccInclude) for t in ds]
		for dirName in incList:
			cipFile.write('-i "' + dirName + '"\n')
			cipFile.write('-q "' + dirName + '"\n')
		
		# STM32 include directory
		stmInclude = os.path.join(KeilInstall, "Inc", "ST", "STM32F10x")
		cipFile.write('-i "' + stmInclude + '"\n')
		cipFile.write('-q "' + stmInclude + '"\n')
		incList = [os.path.join(d, t) for d, ds, fs in os.walk(stmInclude) for t in ds]
		for dirName in incList:
			cipFile.write('-i "' + dirName + '"\n')
			cipFile.write('-q "' + dirName + '"\n')
		
		# CMSIS include directory
		cmsisInclude = os.path.join(KeilInstall, "CMSIS", "Include")
		cipFile.write('-i "' + cmsisInclude + '"\n')
		cipFile.write('-q "' + cmsisInclude + '"\n')
		incList = [os.path.join(d, t) for d, ds, fs in os.walk(cmsisInclude) for t in ds]
		for dirName in incList:
			cipFile.write('-i "' + dirName + '"\n')
			cipFile.write('-q "' + dirName + '"\n')
		
		# RV31 include directory
		rvInclude = os.path.join(KeilInstall, "RV31", "INC")
		cipFile.write('-i "' + rvInclude + '"\n')
		cipFile.write('-q "' + rvInclude + '"\n')
		incList = [os.path.join(d, t) for d, ds, fs in os.walk(rvInclude) for t in ds]
		for dirName in incList:
			cipFile.write('-i "' + dirName + '"\n')
			cipFile.write('-q "' + dirName + '"\n')
		cipFile.close()
	except IOError: pass
	
if __name__=="__main__":
	if sys.platform == 'win32':
		sysInclude()
		
