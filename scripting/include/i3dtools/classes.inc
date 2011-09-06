#if defined _classes_included
	#endinput
#endif
#define _classes_included 

new Handle:classes_CvarKvFilePath;
new Handle:classes_KvFile;
#define _CLASSES_STRING_LENGTH 64
new String:classes_KvFilePath[_CLASSES_STRING_LENGTH];
#define _MAX_CLASSES 32
new String:classes_ClassName[_CLASSES_STRING_LENGTH][_MAX_CLASSES];
new classes_ClassId[_MAX_CLASSES];

/**
 * Initialize the classes part of this plugin
 */
public Classes_OnPluginStart(){
	classes_CvarKvFilePath = CreateConVar("sm_classes_kv_file", "addons/sourcemod/configs/i3dtools/classes.kv", "The location of the KeyValue map file relative to game root");
	RegConsoleCmd("sm_classes", Classes_BuildClassMenu, "Creates a menu to select a player class");
	if(!Classes_ReadKvFile()){
		LogError("Failed to read from KV config at %s", classes_KvFilePath);
	}
}

/**
 * Close all handles on unload to avoid a memory leak
 */
public Classes_OnPluginEnd(){
	CloseHandle(classes_KvFile);
	CloseHandle(classes_CvarKvFilePath);
}

/**
 * Read the specified KV config file
 * 
 * @return true on success, false otherwise
 */
public bool:Classes_ReadKvFile(){
	GetConVarString(classes_CvarKvFilePath, classes_KvFilePath, _CLASSES_STRING_LENGTH);
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
		KvGetSectionName(classes_KvFile, classes_ClassName[index], _CLASSES_STRING_LENGTH);
		// Read the rest of this class's values
		Classes_ReadKvClassNode(classes_KvFile, index);
		// Repeat for each class
		index++;
	} while (KvGotoNextKey(classes_KvFile));
	return true
}

/**
 * Reads the given class config info into local variables
 */
Classes_ReadKvClassNode(const Handle:kv, const index){
	classes_ClassId[index] = KvGetNum(kv, "class_id");
}

public Action:Classes_BuildClassMenu(client, args){
	return Plugin_Handled;
}
