#ifndef FBXPARSER_H
#define FBXPARSER_H

#include "common.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

class FbxParser
{
public:
	FbxParser(FbxString fbxFile);	//ctor
	~FbxParser();

	FbxParser(const FbxParser&) = delete;	//copy constructor/operator= is forbidden
	FbxParser& operator=(const FbxParser&) = delete;
	
	FbxManager* getFbxManager(){ return pManager; }	//get FbxManager
	FbxScene* getFbxScene(){ return pScene; }	//get FbxScene
	FbxVector4* getFbxVector4(){ return controlPoints; }	//get control points of the fbx model
	FbxMesh* getFbxMesh(){ return pMesh; }		//get FbxMesh
	void setFbxMesh(FbxMesh *mesh){ this->pMesh = pMesh; }	//set FbxMesh, it's useless actually

	bool loadScene();		//load scene,return false if failed
	void displayMetaData(FbxScene *pScene);		//display meta data
	void displayGlobalLightSettings(FbxGlobalSettings *pGlobalSettings);		//display global light settings
	void displayHierarchy(FbxScene *pScene);		//display hierarchy of the fbx model
	void displayContent(FbxScene *pScene);		//display info of all the nodes, such as mesh, skeleton, marker, etc


private:
	FbxManager *pManager;
	FbxScene *pScene;
	FbxMesh *pMesh;
	FbxString fbxFile;

	FbxVector4 *controlPoints;
	FbxVector4 *polygonPoints;

	void initFbxObjects();	//initialize FbxManage, FbxScene,etc

	void displayHierarchy(FbxNode *node, int depth);	//used for displayHierarchy(FbxScene*)

	void displayContent(FbxNode *node);			//used for displayContent(FbxScene*)
	//functions below are called inside of displayContent
	void displayMarker(FbxNode *node);				
	void displaySkeleton(FbxNode *node);
	void displayMesh(FbxNode *node);
	void displayTexture(FbxNode *node);
};
 
#endif 