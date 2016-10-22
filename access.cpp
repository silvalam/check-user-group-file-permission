#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

// Check if group flag provided
bool isGroup(char* flag){
	string gflag = "-g";
	string input = string(flag);

	if(gflag == input){
		return true;
	}
	else{
		return false;
	}
}

// Linux does not allow all numeric username
// Check if name is a uid/gid or a username/groupname
bool isID(char* name){
	string input = string(name);
	int i = 0;

	while(i < input.length()){
		if(isdigit(input[i++])){
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}

// Check if file exists
bool fileExists(const char* file){
	struct stat buf;
	return (stat(file, &buf) == 0);
}

// Print if file does not exist
void printNoFile(char* file){
	cout << string(file) << ": No such file or directory" << endl;
}

// Print when the uid or gid is not a valid id, just print the "Other" permissions of file
void getOtherFilePermissions(int id, char* file, bool isGroup){
	int filedes = 0;
	bool isFile = true;
	struct stat st;
	bool canRead = false;
	bool canWrite = false;
	bool canExecute = false;
	int index = 0;

	filedes = open(file, O_RDONLY);

	if(filedes == -1){
		cout << "File failed to open" << endl;
		exit(1);
	}

	if(fstat(filedes, &st)){
		cout << "fstat() error" << endl;
		close(filedes);
		exit(1);
	}

	if(S_ISDIR(st.st_mode)){
		isFile = false;
	}

	if(st.st_mode & S_IROTH){
		canRead = true;
	}
	
	if(st.st_mode & S_IWOTH){
		canWrite = true;
	}
		
	if(st.st_mode & S_IXOTH){
		canExecute = true;
	}

	close(filedes);

	if(canRead && !canWrite && !canExecute){
		index = 1;
	}
	else if(!canRead && canWrite && !canExecute){
		index = 2;
	}
	else if(!canRead && !canWrite && canExecute){
		index = 3;
	}
	else if(canRead && canWrite && !canExecute){
		index = 4;
	}
	else if(canRead && !canWrite && canExecute){
		index = 5;
	}
	else if(!canRead && canWrite && canExecute){
		index = 6;
	}
	else if(canRead && canWrite && canExecute){
		index = 7;
	}
	else if(!canRead && !canWrite && !canExecute){
		index = 8;
	}

	if(!isGroup && isFile){
		switch(index){
			case 1: cout << "The user with UID " << id << " can read the file " << string(file) << endl; break;
			case 2: cout << "The user with UID " << id << " can write the file " << string(file) << endl; break;
			case 3: cout << "The user with UID " << id << " can execute the file " << string(file) << endl; break;
			case 4: cout << "The user with UID " << id << " can read and write the file " << string(file) << endl; break;
			case 5: cout << "The user with UID " << id << " can read and execute the file " << string(file) << endl; break;
			case 6: cout << "The user with UID " << id << " can write and execute the file " << string(file) << endl; break;
			case 7: cout << "The user with UID " << id << " can read, write, and execute the file " << string(file) << endl; break;
			case 8: cout << "The user with UID " << id << " can do nothing with the file " << string(file) << endl; break;
		}
	}
	else if(!isGroup && !isFile){
		switch(index){
			case 1: cout << "The user with UID " << id << " can list the contents of the directory " << string(file) << endl; break;
			case 2: cout << "The user with UID " << id << " can modify the directory " << string(file) << endl; break;
			case 3: cout << "The user with UID " << id << " can search the directory " << string(file) << endl; break;
			case 4: cout << "The user with UID " << id << " can list the contents of and modify the directory " << string(file) << endl; break;
			case 5: cout << "The user with UID " << id << " can list the contents of and search the directory " << string(file) << endl; break;
			case 6: cout << "The user with UID " << id << " can modify and search the the directory " << string(file) << endl; break;
			case 7: cout << "The user with UID " << id << " can list the contents of, modify, and search the directory " << string(file) << endl; break;
			case 8: cout << "The user with UID " << id << " can do nothing with the directory " << string(file) << endl; break;
		}
	}
	else if(isGroup && isFile){
		switch(index){
			case 1: cout << "Members of the group with GID " << id << " can read the file " << string(file) << endl; break;
			case 2: cout << "Members of the group with GID " << id << " can write the file " << string(file) << endl; break;
			case 3: cout << "Members of the group with GID " << id << " can execute the file " << string(file) << endl; break; 
			case 4: cout << "Members of the group with GID " << id << " can read and write the file " << string(file) << endl; break;
			case 5: cout << "Members of the group with GID " << id << " can read and execute the file " << string(file) << endl; break;
			case 6: cout << "Members of the group with GID " << id << " can write and execute the file " << string(file) << endl; break;
			case 7: cout << "Members of the group with GID " << id << " can read, write, and execute the file " << string(file) << endl; break;
			case 8: cout << "Members of the group with GID " << id << " can do nothing with the file " << string(file) << endl; break;
		}
	} 
	else if(isGroup && !isFile){
		switch(index){	
			case 1: cout << "Members of the group with GID " << id << " can list the contents of the directory " << string(file) << endl; break;
			case 2: cout << "Members of the group with GID " << id << " can modify the directory " << string(file) << endl; break;
			case 3: cout << "Members of the group with GID " << id << " can search the directory " << string(file) << endl; break;
			case 4: cout << "Members of the group with GID " << id << " can list the contents of and modify the directory " << string(file) << endl; break;
			case 5: cout << "Members of the group with GID " << id << " can list the contents of and search the directory " << string(file) << endl; break;
			case 6: cout << "Members of the group with GID " << id << " can modify and search the directory " << string(file) << endl; break;
			case 7: cout << "Members of the group with GID " << id << " can list the contents of, modify, and search the directory " << string(file) << endl; break;
			case 8: cout << "Members of the group with GID " << id << " can do nothing with the directory " << string(file) << endl; break;
		}
	}
}

// Print the permissions of the user
// isFile: true if file, false if dir
// isGroup: true if group, false if user 
void printNamePermissions(passwd* user_info, bool isFile, bool canRead, bool canWrite, bool canExecute, char* file){
	char* user_name = user_info->pw_name;
	int uid = user_info->pw_uid;	
	bool noName = false;
	int index = 0;

	// if uid has no name
	if(!user_name){
		noName = true;
	}
	
	// if user can only read file
	if(canRead && !canWrite && !canExecute){
		index = 1;
	}
	else if(!canRead && canWrite && !canExecute){ // else if user can only write file
		index = 2;
	}
	else if(!canRead && !canWrite && canExecute){ // else if user can only execute file
		index = 3;
	}
	else if(canRead && canWrite && !canExecute){ // else if user can read and write file
		index = 4;
	}
	else if(canRead && !canWrite && canExecute){ // else if user can read and execute file
		index = 5;
	}
	else if(!canRead && canWrite && canExecute){ // else if user can write and execute file
		index = 6;
	}
	else if(canRead && canWrite && canExecute){ // else if user can read write and execute file
		index = 7;
	}
	else if(!canRead && !canWrite && !canExecute){ // else if user can do nothing to file
		index = 8;
	}

	if(noName && isFile){
		switch(index){
			case 1: cout << "The user with UID " << uid << " can read the file " << string(file) << endl; break;
			case 2: cout << "The user with UID " << uid << " can write the file " << string(file) << endl; break;
			case 3: cout << "The user with UID " << uid << " can execute the file " << string(file) << endl; break;
			case 4: cout << "The user with UID " << uid << " can read and write the file " << string(file) << endl; break;
			case 5: cout << "The user with UID " << uid << " can read and execute the file " << string(file) << endl; break;
			case 6: cout << "The user with UID " << uid << " can write and execute the file " << string(file) << endl; break;
			case 7: cout << "The user with UID " << uid << " can read, write, and execute the file " << string(file) << endl; break;
			case 8: cout << "The user with UID " << uid << " can do nothing with the file " << string(file) << endl; break;
		}	
	}
	else if(!noName && isFile){
		switch(index){
			case 1: cout << "The user " << string(user_name) << " (UID " << uid << ") can read the file " << string(file) << endl; break;
			case 2: cout << "The user " << string(user_name) << " (UID " << uid << ") can write the file " << string(file) << endl; break;
			case 3: cout << "The user " << string(user_name) << " (UID " << uid << ") can execute the file " << string(file) << endl; break;
			case 4: cout << "The user " << string(user_name) << " (UID " << uid << ") can read and write the file " << string(file) << endl; break;
			case 5: cout << "The user " << string(user_name) << " (UID " << uid << ") can read and execute the file " << string(file) << endl; break;
			case 6: cout << "The user " << string(user_name) << " (UID " << uid << ") can write and execute the file " << string(file) << endl; break;
			case 7: cout << "The user " << string(user_name) << " (UID " << uid << ") can read, write, and execute the file " << string(file) << endl; break;
			case 8: cout << "The user " << string(user_name) << " (UID " << uid << ") can do nothing with the file " << string(file) << endl; break;
		}
	}
	else if(noName && !isFile){
		switch(index){
			case 1: cout << "The user with UID " << uid << " can list the contents of the directory " << string(file) << endl; break;
			case 2:	cout << "The user with UID " << uid << " can modify the directory " << string(file) << endl; break;
			case 3: cout << "The user with UID " << uid << " can search the directory " << string(file) << endl; break;
			case 4: cout << "The user with UID " << uid << " can list the contents of and modify the directory " << string(file) << endl; break;
			case 5: cout << "The user with UID " << uid << " can list the contents of and search the directory " << string(file) << endl; break;
			case 6: cout << "The user with UID " << uid << " can modify and search the directory " << string(file) << endl; break;
			case 7: cout << "The user with UID " << uid << " can list the contents of, modify, and search the directory " << string(file) << endl; break;
			case 8: cout << "The user with UID " << uid << " can do nothing with the directory " << string(file) << endl; break;
		}
	}
	else if(!noName && !isFile){
		switch(index){
			case 1: cout << "The user " << string(user_name) << " (UID " << uid << ") can list the contents of the directory " << string(file) << endl; break;
			case 2: cout << "The user " << string(user_name) << " (UID " << uid << ") can modify the directory " << string(file) << endl; break;
			case 3: cout << "The user " << string(user_name) << " (UID " << uid << ") can search the directory " << string(file) << endl; break;
			case 4: cout << "The user " << string(user_name) << " (UID " << uid << ") can list the contents of and modify the directory " << string(file) << endl; break;
			case 5: cout << "The user " << string(user_name) << " (UID " << uid << ") can list the contents of and search the directory " << string(file) << endl; break;
			case 6: cout << "The user " << string(user_name) << " (UID " << uid << ") can modify and search the directory " << string(file) << endl; break;
			case 7: cout << "The user " << string(user_name) << " (UID " << uid << ") can list the contents of, modify, and search the directory " << string(file) << endl; break;
			case 8: cout << "The user " << string(user_name) << " (UID " << uid << ") can do nothing with the directory " << string(file) << endl; break;
		}
	}
}

// Print the permissions of group
void printGroupPermissions(struct group* group_info, bool isFile, bool canRead, bool canWrite, bool canExecute, char* file){	
	char* group_name = group_info->gr_name;
	int gid = group_info->gr_gid;
	bool noName = false;
	int index = 0;

	// if gid has no name
	if(!group_name){
		noName = true;
	}

	if(canRead && !canWrite && !canExecute){
		index = 1;
	}
	else if(!canRead && canWrite && !canExecute){
		index = 2;
	}
	else if(!canRead && !canWrite && canExecute){
		index = 3;
	}
	else if(canRead && canWrite && !canExecute){
		index = 4;
	}
	else if(canRead && !canWrite && canExecute){
		index = 5;
	}
	else if(!canRead && canWrite && canExecute){
		index = 6;
	}
	else if(canRead && canWrite && canExecute){
		index = 7;
	}
	else if(!canRead && !canWrite && !canExecute){
		index = 8;
	}

	if(noName && isFile){
		switch(index){
			case 1: cout << "Members of the group with GID " << gid << " can read the file " << string(file) << endl; break;
			case 2: cout << "Members of the group with GID " << gid << " can write the file " << string(file) << endl; break;
		 	case 3: cout << "Members of the group with GID " << gid << " can execute the file " << string(file) << endl; break;
			case 4: cout << "Members of the group with GID " << gid << " can read and write the file " << string(file) << endl; break;
			case 5: cout << "Members of the group with GID " << gid << " can read and execute the file " << string(file) << endl; break;
			case 6: cout << "Members of the group with GID " << gid << " can write and execute the file " << string(file) << endl; break;
			case 7: cout << "Members of the group with GID " << gid << " can read, write, and execute the file " << string(file) << endl; break;
			case 8: cout << "Members of the group with GID " << gid << " can do nothing with the file " << string(file) << endl; break;
		}
	}
	else if(!noName && isFile){
		switch(index){
			case 1: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can read the file " << string(file) << endl; break;
			case 2: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can write the file " << string(file) << endl; break;
			case 3: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can execute the file " << string(file) << endl; break;
			case 4: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can read and write the file " << string(file) << endl; break;
			case 5: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can read and execute the file " << string(file) << endl; break;
			case 6: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can write and execute the file " << string(file) << endl; break;
			case 7: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can read, write, and execute the file " << string(file) << endl; break;
			case 8: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can do nothing with the file " << string(file) << endl; break;
		}
	}
	else if(noName && !isFile){
		switch(index){	
			case 1: cout << "Members of the group with GID " << gid << " can list the contents of the directory " << string(file) << endl; break;
			case 2: cout << "Members of the group with GID " << gid << " can modify the directory " << string(file) << endl; break;
			case 3: cout << "Members of the group with GID " << gid << " can search the directory " << string(file) << endl; break;
			case 4: cout << "Members of the group with GID " << gid << " can list the contents of and modify the directory " << string(file) << endl; break;
			case 5: cout << "Members of the group with GID " << gid << " can list the contents of and search the directory " << string(file) << endl; break;
			case 6: cout << "Members of the group with GID " << gid << " can modify and search the directory " << string(file) << endl; break;	
			case 7: cout << "Members of the group with GID " << gid << " can list the contents of, modify, and search the directory " << string(file) << endl; break;
			case 8: cout << "Members of the group with GID " << gid << " can do nothing with the directory " << string(file) << endl; break;
		}
	}	
	else if(!noName && !isFile){
		switch(index){
			case 1: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can list the contents of the directory " << string(file) << endl; break;
			case 2: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can modify the directory " << string(file) << endl; break;
			case 3: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can search the directory " << string(file) << endl; break;
			case 4: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can list the contents of and modify the directory " << string(file) << endl; break;
			case 5: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can list the contents of and search the directory" << string(file) << endl; break;
			case 6: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can modify and search the directory " << string(file) << endl; break;
			case 7: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can list the contents of, modify, and search the directory " << string(file) << endl; break;
			case 8: cout << "Members of the group " << string(group_name) << " (GID " << gid << ") can do nothing with the directory " << string(file) << endl; break;
		}
	}
}

// Get the permissions of the user before printing
void getNamePermissions(passwd* user_info, char* file){
	int filedes = 0;
	struct stat st; 	
	bool isFile = true;
	bool canRead = false;
	bool canWrite = false;
	bool canExecute = false;
	bool isOwner = false;

	// open file
	filedes = open(file, O_RDONLY);

	// check if file opened unsuccessfully
	if(filedes == -1){
		cout << "File failed to open" << endl;
		exit(1);
	}

	// if fstat() fails
	if(fstat(filedes, &st)){
		cout << "fstat() error" << endl;
		close(filedes);
		exit(1);
	}

	// if file is a directory
	if(S_ISDIR(st.st_mode)){
		isFile = false;
	}

	// check if user is the owner of this file
	if(user_info->pw_uid == st.st_uid){
		isOwner = true;
	}

	// Check user or other permissions
	if(isOwner){
		if(st.st_mode & S_IRUSR){
			canRead = true;
		}

		if(st.st_mode & S_IWUSR){
			canWrite = true;
		}

		if(st.st_mode & S_IXUSR){
			canExecute = true;
		}
	}
	else{
		if(st.st_mode & S_IROTH){
			canRead = true;
		}

		if(st.st_mode & S_IWOTH){
			canWrite = true;
		}

		if(st.st_mode & S_IXOTH){
			canExecute = true;
		}	
	}

	printNamePermissions(user_info, isFile, canRead, canWrite, canExecute, file);

	close(filedes);
}

// Get the permissions of group before printing
void getGroupPermissions(group* group_info, char* file){
	int filedes = 0;
	struct stat st;
	bool isFile = true;
	bool canRead = false;
	bool canWrite = false;
	bool canExecute = false;
	bool isOwner = false;

	// open file
	filedes = open(file, O_RDONLY);

	// check if file opened unsuccessfully
	if(filedes == -1){
		cout << "File failed to open" << endl;
		exit(1);
	}	
	
	// if fstat() fails
	if(fstat(filedes, &st)){
		cout << "fstat() error" << endl;
		close(filedes);
		exit(1);
	}

	// if file is a directory
	if(S_ISDIR(st.st_mode)){
		isFile = false;
	}

	// check if group is the group owner of file
	if(group_info->gr_gid == st.st_gid){
		isOwner = true;
	}

	// check group or other permissions
	if(isOwner){
		if(st.st_mode & S_IRGRP){
			canRead = true;
		}
			
		if(st.st_mode & S_IWGRP){
			canWrite = true;
		}	

		if(st.st_mode & S_IXGRP){
			canExecute = true;
		}
	}
	else {
		if(st.st_mode & S_IROTH){
			canRead = true;
		}
		
		if(st.st_mode & S_IWOTH){
			canWrite = true;
		}

		if(st.st_mode & S_IXOTH){
			canExecute = true;
		}
	}

	printGroupPermissions(group_info, isFile, canRead, canWrite, canExecute, file);
	close(filedes);
}

// Set up logic on how to access file via name or via id as user
void setupName(char* name, char* file){
	struct passwd* user_info;
	bool gotOtherFilePerm = false;

	// if name is uid
	if(isID(name)){
		if(!fileExists(file)){
			printNoFile(file);
			exit(1);
		}

		// if uid does not exist then name has Other permissions on file
		if(!getpwuid(atoi(name))){
			getOtherFilePermissions(atoi(name), file, false);
			gotOtherFilePerm = true;
		}
		else { // else if uid exists, get user info 
			user_info = getpwuid(atoi(name));
		}
	}
	else { // else check if user name exists
		user_info = getpwnam(name);

		// if user name does not exist		
		if(!user_info){
			cout << string(name) << ": no such user" << endl;
			exit(1);
		}
		else {
			// if file does not exist
			if(!fileExists(file)){
				printNoFile(file);
				exit(1);
			}
		}
	}
	
	if(!gotOtherFilePerm){
		getNamePermissions(user_info, file);
	}
}

// Set up logic on how to open file via name or id as group
void setupGroup(char* name, char* file){
	struct group* group_info;
	bool gotOtherFilePerm = false;

	// if name is gid
	if(isID(name)){
		if(!fileExists(file)){
			printNoFile(file);
			exit(1);
		}
		
		// if gid does not exist then they can do nothing
		if(!getgrgid(atoi(name))){
			getOtherFilePermissions(atoi(name), file, true);
			gotOtherFilePerm = true;
		}
		else { // else if gid exists, set group info
			group_info = getgrgid(atoi(name));
		}
	}
	else { // else check if account group exists
		group_info = getgrnam(name);
		
		// if group does not exist
		if(!group_info){
			cout << string(name) << ": no such group" << endl;
			exit(1);
		}
		else { // else if group does exist
			// if file does not exist
			if(!fileExists(file)){
				printNoFile(file);
				exit(1);
			}
		}
	}
	
	if(!gotOtherFilePerm){
		getGroupPermissions(group_info, file);
	}
}

int main(int argc, char* argv[]){
	int itr = 0; 
	
	// if gflag names are groups
	if(isGroup(argv[1])){
		itr = 3;
		while(itr < argc){
			setupGroup(argv[2], argv[itr++]);
		}	
	}
	else { // else names are users
		itr = 2; 
		while(itr < argc){
			setupName(argv[1], argv[itr++]);
		}		
	}

	return 0;
}
