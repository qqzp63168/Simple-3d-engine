#include "StdAfx.h"
#include "GameScene.h"
#include<sstream>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

//全局变量

int x;//测试用
SceneObjectList *pOList;//场景中对象列表
LPDIRECT3D9             g_pD3D = NULL; // 用来创建d3d设备 
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // 渲染设备
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; //   Vertices缓冲

LPD3DXMESH          g_pMesh = NULL; // Our mesh object in sysmem
D3DMATERIAL9*       g_pMeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; // Textures for our mesh
DWORD               g_dwNumMaterials = 0L;   // Number of mesh materials


// vertex类型结构体
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // vertex转换格式
    DWORD color;        // vertex颜色
};

// 定义fvf
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


//公共函数声明

//全局函数
static int InitSourceSceneObject(lua_State *L);//元场景对象初始化

//lua全局函数跳板函数
static int createSceneObject(lua_State* L) ;
static int addSceneObject(lua_State* L);
static int createShap(lua_State* L);
//lua成员函数跳板函数
static int DestorySceneObject(lua_State* L);
static int B_SceneObject_setName(lua_State* L);
static int B_SceneObject_setLuaPath(lua_State* L);
static int B_SceneObject_setLuaPathc(lua_State* L);
static int B_SceneObject_setLocation(lua_State* L);
static int B_SceneObject_getControlInfo(lua_State* L);
static int a(lua_State* L);

//d3d函数
HRESULT InitD3D( HWND hWnd );
HRESULT InitVB();
VOID Cleanup();
int RenderMesh(LPD3DXMESH mesh,D3DMATERIAL9* pMeshMaterials,LPDIRECT3DTEXTURE9* pMeshTextures,DWORD dwNumMaterials,D3DXMATRIXA16* pMatWorld);//模型，贴图，mesh子集个数，转换矩阵
HRESULT InitGeometry();
VOID SetupMatrices();
shap* InitShap(string smodpath);
GameScene::GameScene(void)
{	x=10;
	rendercs=0;//渲染测试
	
	pOList=new SceneObjectList();
	pSLua = luaL_newstate();
	InitSourceSceneObject(pSLua);//初始化场景对象
	//初始化d3d
	InitD3D( Hwnd );
	//InitGeometry();
		CI.ctrl=0;
	CI.down=0;
	CI.left=0;
	CI.right=0;
	CI.space=0;
	CI.up=0;

}
GameScene::GameScene(HWND hwnd)
{
	x=1;
	rendercs=0;//渲染测试
	
	pOList=new SceneObjectList();
	pSLua = luaL_newstate();
	InitSourceSceneObject(pSLua);//初始化场景对象
		//初始化d3d
	InitD3D( hwnd );
	//InitGeometry();
	CI.ctrl=0;
	CI.down=0;
	CI.left=0;
	CI.right=0;
	CI.space=0;
	CI.up=0;
}

GameScene::~GameScene(void)
{
	 lua_close(pSLua);
	 Cleanup();//清楚d3d对象
}

int GameScene::render()
{
	
	D3DXMATRIXA16* pmat=new D3DXMATRIXA16;
	D3DXMatrixTranslation(pmat,0,0,0);
	
	ostringstream oss;
	oss<<rendercs*x;
	TextOutA(Hwdc,50,50,oss.str().c_str(),5);
	rendercs++;
	//测试SceneObject对象run
	pOList->resetIterator();

	do
	{    
		SceneObject*pSOm=pOList->getObject();
		if(NULL==pSOm)
		{
			break;
		}
		//渲染场景的位置-------------------------------------------------------------------------------------------------------------------
		if(pSOm->pShap!=NULL)
		{
			RenderMesh(pSOm->pShap->mesh,pSOm->pShap->pMeshMaterials,pSOm->pShap->pMeshTextures,pSOm->pShap->dwNumMaterials,&(pSOm->mMat));
		}
		
		TextOutA(Hwdc,50,100,pOList->getObject()->sName.c_str(),5);
		ostringstream oss2;
		oss2<<pOList->L_SceneObject.size();
		TextOutA(Hwdc,50,150,oss2.str().c_str(),5);
	}while(NULL!=pOList->nextObject());

	//lua测试
	/*
	  luaL_openlibs(pSLua);
	  lua_pushcfunction(pSLua, luatestGlue);
      lua_setglobal(pSLua, "luatestGlue");

	  luaL_dofile(pSLua,"testlua.lua");

	  TextOutA(Hwdc,50,50,,5);
	*/
	
	return 1;
}
void GameScene::getControl(int iMes)
{
	switch(iMes)
		{
	    case VK_RIGHT:CI.right=1;break;
		case VK_LEFT:CI.left=1;break;
		case VK_UP:CI.up=1;break;
		case VK_DOWN:CI.down=1;break;
		case VK_SPACE:CI.space=1;break;
		case VK_CONTROL:CI.ctrl=(CI.ctrl+1)%4+10;break;
		default:break;
		}
	pOList->resetIterator();
	do
	{
		if(NULL==pOList->getObject())
		{
			break;
		}
		
		pOList->getObject()->getControl(CI);

	}while(NULL!=pOList->nextObject());


}
void GameScene::getControlDown(int iMes)
{
	switch(iMes)
		{
	    case VK_RIGHT:CI.right=0;break;
		case VK_LEFT:CI.left=0;break;
		case VK_UP:CI.up=0;break;
		case VK_DOWN:CI.down=0;break;
		case VK_SPACE:CI.space=0;break;
		case VK_CONTROL:CI.ctrl=(CI.ctrl+1)%4+10;break;
		default:break;
		}
	pOList->resetIterator();
	do
	{
		if(NULL==pOList->getObject())
		{
			break;
		}
		
		pOList->getObject()->getControl(CI);

	}while(NULL!=pOList->nextObject());


}




int GameScene::runScene()
{
	//轮训场景对象
	pOList->resetIterator();
	do
	{
		if(NULL==pOList->getObject())
		{
			break;
		}
		luaL_dofile(pSLua,pOList->getObject()->luapathc.c_str());//执行脚本
		luaL_dofile(pSLua,pOList->getObject()->luapath.c_str());//执行脚本

	}while(NULL!=pOList->nextObject());


	

	return 1;
}

//公共全局函数---------------------------------------------------------------------------------------------------------------------公共全局函数

//元场景对象初始化
static int InitSourceSceneObject(lua_State *L)
{
	
    luaopen_base(L); 
	 
	//全局函数注册------------------------------------------------------------------------------------------------------------------------全局函数注册
	lua_pushcfunction(L, createSceneObject);    // 注册用于创建SceneObject类的全局函数 createSceneObject()
    lua_setglobal(L,  "createSceneObject");

	lua_pushcfunction(L, addSceneObject);    // 用于添加对象 addSceneObject(sceneObject)
    lua_setglobal(L,  "addSceneObject");
	
	lua_pushcfunction(L, createShap);    // 用于创建shap给sceneObjet    createShap(string .x路径,sceneObjet*) 
    lua_setglobal(L,  "createShap");



	//成员函数注册准备
	luaL_newmetatable(L, "SceneObject");           // 创建一个元表
    lua_pushstring(L, "__gc");                    // 垃圾回收
    lua_pushcfunction(L, DestorySceneObject);
    lua_settable(L, -3);                               // 公共函数调用的实现就在此啊

	 lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);                           // 注意这一句，其实是将__index设置成元表自己
    lua_settable(L, -3);
    //成员函数注册------------------------------------------------------------------------------------------------------------------------成员函数注册
    lua_pushstring(L, "setName");                     // 放元表中增加一个函数。 setName(string sss);
    lua_pushcfunction(L, B_SceneObject_setName);
    lua_settable(L, -3);
	//test a
	 lua_pushstring(L, "a");                     // 放元表中增加一个函数。
    lua_pushcfunction(L, a);
    lua_settable(L, -3);
	//设置luapath
	 lua_pushstring(L, "setLuaPath");                     // 放元表中增加一个函数。setPath(string sss);
    lua_pushcfunction(L, B_SceneObject_setLuaPath);
    lua_settable(L, -3);
	//设置luapath
	 lua_pushstring(L, "setLuaPathc");                     // 放元表中增加一个函数。setPathc(string sss);
    lua_pushcfunction(L, B_SceneObject_setLuaPathc);
    lua_settable(L, -3);
	//设置位置
	lua_pushstring(L, "setLocation");                     // 放元表中增加一个函数。setLocation(float x,float y,float z);
    lua_pushcfunction(L, B_SceneObject_setLocation);
    lua_settable(L, -3);

	//得到控制信息
	lua_pushstring(L, "getControlInfo");                     // 放元表中增加一个函数。getControlInfo()
	lua_pushcfunction(L, B_SceneObject_getControlInfo);
    lua_settable(L, -3);

	lua_pop(L,-1);

	luaL_dofile(L, "InitScene.lua");  
	return 1;
}
//lua全局函数------------------------------------------------------------------------------------------------------------------------------------------lua全局函数

//创建table（元表为SceneObject，类似类的实例化）(会在栈里留下一个userdata是对象指针)
static int createSceneObject(lua_State* L)
{
    // 创建一个元表为CTest的Table——Lua对象
    *(SceneObject**)lua_newuserdata(L, sizeof(SceneObject*)) = new SceneObject();
    luaL_getmetatable(L, "SceneObject");
    lua_setmetatable(L, -2);

    return 1;
}
//创建一个shap
static int createShap(lua_State* L)
{
	string path=lua_tostring(L,-2);
	shap* pShap;
	pShap=InitShap(path);//初始化shap
	(*(SceneObject**)lua_topointer(L,-1))->pShap=pShap;
	//*(shap**)lua_newuserdata(L, sizeof(SceneObject*)) = pShap;
    //luaL_getmetatable(L, "shap");
   // lua_setmetatable(L, -3);
	return 1;
}
//添加场景对象
static int addSceneObject(lua_State* L)
{
	if(*(SceneObject**)lua_topointer(L,-1)!=NULL)
	{
		SceneObject* pso=*(SceneObject**)lua_topointer(L,-1);
		pOList->addObject(pso);
	}
	
		return 1;
}



//成员函数跳板函数------------------------------------------------------------------------------------------------------------------------------------成员函数跳板函数

//销毁对象
static int DestorySceneObject(lua_State* L)
{
    // 释放对象
    delete *(SceneObject**)lua_topointer(L, 1);
    return 0;
}


//设置name
static int B_SceneObject_setName(lua_State* L)
{
	SceneObject* pSO=*(SceneObject**)lua_topointer(L,-2);
	pSO->sName=lua_tostring (L, -1);
	return 1;

}

//设置luapath
static int B_SceneObject_setLuaPath(lua_State* L)
{
	SceneObject* pSO=*(SceneObject**)lua_topointer(L,-2);
	pSO->luapath=lua_tostring (L, -1);
	return 1;

}
//设置luapathc
static int B_SceneObject_setLuaPathc(lua_State* L)
{
	SceneObject* pSO=*(SceneObject**)lua_topointer(L,-2);
	pSO->luapathc=lua_tostring (L, -1);
	return 1;

}
//设置位置
static int B_SceneObject_setLocation(lua_State* L)
{
	SceneObject* pSO=*(SceneObject**)lua_topointer(L,-4);
	pSO->setLocation(lua_tonumber(L,-3),lua_tonumber(L,-2),lua_tonumber(L,-1));
	return 1;

}
static int B_SceneObject_getControlInfo(lua_State* L)
{
	SceneObject* pSO=*(SceneObject**)lua_topointer(L,-1);
	lua_newtable(L);
	lua_pushstring(L,"up");
	lua_pushnumber(L,pSO->m_CI.up);
	lua_settable(L,-3);
	lua_pushstring(L,"down");
	lua_pushnumber(L,pSO->m_CI.down);
	lua_settable(L,-3);
	lua_pushstring(L,"left");
	lua_pushnumber(L,pSO->m_CI.left);
	lua_settable(L,-3);
	lua_pushstring(L,"right");
	lua_pushnumber(L,pSO->m_CI.right);
	lua_settable(L,-3);
	lua_pushstring(L,"space");
	lua_pushnumber(L,pSO->m_CI.space);
	lua_settable(L,-3);
	lua_pushstring(L,"ctrl");
	lua_pushnumber(L,pSO->m_CI.ctrl);
	lua_settable(L,-3);
	return 1;
}

static int a(lua_State* L)
{
	SceneObject* pSO=*(SceneObject**)lua_topointer(L,-2);
	return 1;

}
//

//d3d相关函数

HRESULT InitD3D( HWND hWnd )
{

    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;


    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );

    return S_OK;
}

HRESULT InitVB()
{

    CUSTOMVERTEX Vertices[] =
    {
        { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
        { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
        {  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
    };

    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }

   
    VOID* pVertices;
    if( FAILED( g_pVB->Lock( 0, sizeof( Vertices ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices, sizeof( Vertices ) );
    g_pVB->Unlock();

    return S_OK;
}
shap* InitShap(string smodpath)
{
	shap *pShap=new shap;
	

	LPD3DXBUFFER pD3DXMtrlBuffer;
	wstring s=s2ws(smodpath);
    // 装载.x文件，初始化shap的mesh和dwNumMaterials
	if( FAILED( D3DXLoadMeshFromX( s.c_str(), D3DXMESH_SYSTEMMEM,
                                   g_pd3dDevice, NULL,
								   &pD3DXMtrlBuffer, NULL, &pShap->dwNumMaterials,
								   &pShap->mesh ) ) )
    {        
            return NULL;
     
    }



    // 取出materials和textures
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
	pShap->pMeshMaterials = new D3DMATERIAL9[pShap->dwNumMaterials];
    if( pShap->pMeshMaterials == NULL )
        return NULL;
	pShap->pMeshTextures = new LPDIRECT3DTEXTURE9[pShap->dwNumMaterials];
    if( pShap->pMeshTextures == NULL )
        return NULL;

    for( DWORD i = 0; i < pShap->dwNumMaterials; i++ )
    {
        // Copy the material
        pShap->pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        pShap->pMeshMaterials[i].Ambient = pShap->pMeshMaterials[i].Diffuse;
		///////////////////////////////////////////////////////////////////////////////////
        pShap->pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( g_pd3dDevice,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &pShap->pMeshTextures[i] ) ) )
            {
                ;
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

    return pShap;
	
}


HRESULT InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
    if( FAILED( D3DXLoadMeshFromX( L"model\\Tiger.x", D3DXMESH_SYSTEMMEM,
                                   g_pd3dDevice, NULL,
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                   &g_pMesh ) ) )
    {        
            return E_FAIL;
     
    }



    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
    if( g_pMeshMaterials == NULL )
        return E_OUTOFMEMORY;
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
    if( g_pMeshTextures == NULL )
        return E_OUTOFMEMORY;

    for( DWORD i = 0; i < g_dwNumMaterials; i++ )
    {
        // Copy the material
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

        g_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( g_pd3dDevice,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &g_pMeshTextures[i] ) ) )
            {
                
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

    return S_OK;
}


VOID Cleanup()
{
    if( g_pMeshMaterials != NULL )
        delete[] g_pMeshMaterials;

    if( g_pMeshTextures )
    {
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            if( g_pMeshTextures[i] )
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }
    if( g_pMesh != NULL )
        g_pMesh->Release();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

VOID SetupMatrices()
{
    

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the 
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt( 0.0f, 10.0f,0.01f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
int RenderMesh(LPD3DXMESH mesh,D3DMATERIAL9* pMeshMaterials,LPDIRECT3DTEXTURE9* pMeshTextures,DWORD dwNumMaterials,D3DXMATRIXA16* pMatWorld)
{
    
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // 开始渲染 scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
       
        SetupMatrices();

      
        for( DWORD i = 0; i < dwNumMaterials; i++ )
        {
			
			//执行变换矩阵
			
			 g_pd3dDevice->SetTransform( D3DTS_WORLD, pMatWorld );
           
            g_pd3dDevice->SetMaterial( &pMeshMaterials[i] );
            g_pd3dDevice->SetTexture( 0, pMeshTextures[i] );

			mesh->DrawSubset( i );

        }
		 


        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return 1;
}

