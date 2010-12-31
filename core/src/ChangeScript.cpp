#include "ChangeScript.h"

#include <sstream>

const string ChangeScript::UNDO_MARKER = "--//@UNDO";

ChangeScript::ChangeScript(int id, string filename, string description) {
	this->id = id;
	this->filename = filename;
	this->description = description;
}

string ChangeScript::getFilename() {
	return filename;
}

int ChangeScript::getId() {
	return id;
}

string ChangeScript::getDescription() {
	return description;
}

int ChangeScript::compare(shared_ptr<ChangeScript> first, shared_ptr<ChangeScript> second) {
	return first->id < second->id;
}

string ChangeScript::toString() {
    ostringstream oss;
    oss << "#" << id  << ": " << description;
    return oss.str();
}

string ChangeScript::getContent() {
	return getFileContents(false);
}

string ChangeScript::getUndoContent() {
	return getFileContents(true);
}

string ChangeScript::getFileContents(bool onlyAfterUndoMarker) {
	
	// try {
	// 		StringBuilder content = new StringBuilder();
	// 		boolean foundUndoMarker = false;
	// 		BufferedReader reader = new BufferedReader(new FileReader(file));
	//
	// 		try {
	// 			for (;;) {
	// 				String str = reader.readLine();
	//
	// 				if (str == null)
	// 					break;
	//
	// 				if (str.trim().equals(UNDO_MARKER)) {
	// 					foundUndoMarker = true;
	// 					continue;
	// 				}
	//
	// 				if (foundUndoMarker == onlyAfterUndoMarker) {
	// 					content.append(str);
	// 					content.append('\n');
	// 				}
	// 			}
	// 		} finally {
	// 			reader.close();
	// 		}
	//
	// 		return content.toString();
	// 	} catch (IOException e) {
	// 		throw new DbDeployException("Failed to read change script file", e);
	// 	}

	return "";
}
