#if defined _classes_included
	#endinput
#endif
#define _classes_included 

new Handle:classes_CvarKvFilePath;
new Handle:classes_KvFile;
new String:classes_KvFilePath[64];
#define _MAX_CLASSES 32
new String:classes_ClassName[64][_MAX_CLASSES];
new classes_ClassId[64][_MAX_CLASSES];

/**
 * Initialize the classes part of this plugin
 */
public Classes_OnPluginStart(){
	classes_CvarKvFilePath = CreateConVar("sm_classes_kv_file", "cfg/sourcemod/classes.kv");
	RegConsoleCmd("sm_classes", Classes_BuildClassMenu());
	if(!Classes_ReadKvFile()){
		LogError("Failed to read from KV config at %s", classes_KvFilePath);
	}
}

/**
 * Close all handles on unload to avoid a memory leak
 */
public Classes_OnPluginUnload(){
	CloseHandle(classes_KvFile);
	CloseHandle(classes_CvarKvFilePath);
}

/**
 * Read the specified KV config file
 * 
 * @return true on success, false otherwise
 */
public bool:Classes_ReadKvFile(){
	GetConVarString(classes_CvarKvFilePath, classes_KvFilePath, sizeof(classes_KvFilePath));
	// Get the KVs from file
	if(!FileToKeyValues(classes_KvFile, classes_KvFilePath)){
		// Return false if the file wasn't parsed.
		return false;
	}
	
	// Go to the first subkey of the file
	if(!KvGotoFirstSubKey(classes_KvFile)){
		return false;
	}
	new index = 0;
	do
	{
		// Grab the section name as the class name
		KvGetSectionName(classes_KvFile, classes_ClassName[index], sizeof(classes_ClassName[index]));
		// Read the rest of this class's values
		Classes_ReadKvClassNode(classes_KvFile, index);
		// Repeat for each class
	} while (KvGotoNextKey(classes_KvFile));
	return true
}

public Classes_ReadKvClassNode(const Handle:kv, const index){
	
}

public Action:Classes_BuildClassMenu(){
	return Plugin_Handled;
}
