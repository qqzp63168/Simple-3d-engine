#pragma once

#include<sstream>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
using namespace std;

class shap
{
public:
	char cModPath(); 
	LPD3DXMESH          mesh;
	DWORD               dwNumMaterials;	
	D3DMATERIAL9*       pMeshMaterials; // ָ��һ��material����
	LPDIRECT3DTEXTURE9* pMeshTextures; // ָ��һ��Texture����
	string sModPath;
	int outline;
	shap(void);
	

	~shap(void);
};

