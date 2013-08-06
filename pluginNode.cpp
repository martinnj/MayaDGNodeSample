/* This is the example plugin from the autodesk MAya 2013 API.
 * it is found at: http://docs.autodesk.com/MAYAUL/2013/ENU/Maya-API-Documentation/files/Dependency_graph_plugins_The_basics.htm
 * I'm implementing it to learn.
 */


// Basic and plugin related headers, common with command plugins
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <maya/MString.h> 
#include <maya/MFnPlugin.h>

// Node related headers
#include <maya/MPxNode.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

// We only use numeric data so only one extra header is needed
#include <maya/MFnNumericAttribute.h>

// Class for the node, derived from MPxNode.
class sine : public MPxNode
{
public:
	// Functions
	sine();
	virtual ~sine();
	virtual MStatus compute( const MPlug& plug,
		                     MDataBlock& data);
	static void* creator();
	static MStatus Initialize();

	// Attributes
	static MObject input;
	static MObject output;
	static MTypeId id;
};

/* Assign node ID. For actual nodes you should contact Autodesk Technical Support
 * and request a permanent ID. If your ID's collide with another node, a fatal error
 * will occur
 */
MTypeId sine::id(0x80000);

// Initialize attributes to Null.
MObject sine::input;
MObject sine::output;

void* sine::creator() {
	return new sine;
}

// Initialize gets called when the node gets registered with Maya
MStatus sine::Initialize() {
	MFnNumericAttribute nAttr;

	// Create the output attribute with a long name, shortname, float datatype and initial value 0.0.
	output = nAttr.create("output","out",
		                  MFnNumericData::kFloat, 0.0);
	// Since it is the output, it cannot be written to, or stored (waste of space)
	nAttr.setWritable(false);
	nAttr.setStorable(false);

	// Repeat for input, except it's both wriable and storable.
	input = nAttr.create("input","in",
		                 MFnNumericData::kFloat, 0.0);
	nAttr.setStorable(true);

	// Add the attribute to the node, mark it as affecting the output attribute.
	addAttribute(input);
	attributeAffects(input, output);
	addAttribute(output);

	//Node initialized, good job :)
	return MS::kSuccess;
}

// Simple node, so constructor and destructor do nothing.
sine::sine() {}
sine::~sine() {}

// The actual "brain" of the plugin.
MStatus sine::compute( const MPlug& plug, MDataBlock& data ) {
	MStatus returnStatus;

	// Check that the requested plug is the output plug
	if(plug == output) {
		// Get the input data from the input.
		MDataHandle inputData = data.inputValue (input, &returnStatus);

		// If we couldn't get the data, write to console error stream.
		if( returnStatus != MS::kSuccess) {
			cerr << "Error getting input data!" << endl;
		} else {
			// Otherwise, calculate the result.
			float result = sin(inputData.asFloat());
			MDataHandle outputHandle = data.outputValue(output);

			// Put the result in the output and mark the plug as clean (newly recomputed)
			outputHandle.set(result);
			data.setClean(plug);
		}
	}
	return MS::kSuccess;
}

// Like all other plugins, we need to register it with Maya
MStatus initializePlugin( MObject obj ) {
	MStatus status;
	MFnPlugin plugin (obj, "Martin Jørgensen", "1.0", "Any");

	// Note how a node have more aruments for registration than a command.
	status = plugin.registerNode("sine", sine::id, sine::creator, sine::Initialize);

	// return the registration result, if it failed the PLUGIN will NOT be registered!
	return status;
}

// And deregister it, note that deregistration also passes the node ID.
MStatus uninitializePlugin( MObject obj ) {
	MStatus status;
	MFnPlugin plugin( obj );
	status = plugin.deregisterNode(sine::id);
	return status;
}