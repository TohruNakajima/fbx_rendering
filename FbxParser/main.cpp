#include "ModelReconstruct.h"
#include <iostream> 
using namespace std;

FbxParser *parser;
bool gSupportVBO;

int main(int argc, char **argv)
{
	const char* model_rin = "models/rin/rin-opt";
	const char* model_soldier = "soldier";
	parser = new FbxParser(FbxString(model_rin));
	bool loadResult = parser->LoadScene();		//load scene
	if (loadResult) 
	{
		parser->DisplayGlobalLightSettings(&parser->GetFbxScene()->GetGlobalSettings());		//Display global light settings
		parser->DisplayHierarchy(parser->GetFbxScene());		// Display hierarchy of model
		parser->DisplayContent(parser->GetFbxScene());	// Display content
		parser->DisplayPose(parser->GetFbxScene());
	}
	else 
	{
		FBXSDK_printf("error: load scene failed, check if the file exists.\n\n");
		system("pause");
		exit(1);
	}
	// parser->SetAnimStatus(DYNAMICAL);
	if (parser) 
	{
		gSupportVBO = InitOpenGL(argc, argv);	//Initialize the environment of OpenGL
		RunOpenGL();
	}
	system("pause");
}