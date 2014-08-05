sourceObject:setName("Object")


myPlane=createSceneObject()

myPlane:setName("O2")

myPlane:setLuaPath("lua\\myPlane\\myPlane.lua");

createShap("model\\myPlane\\Tiger.x",myPlane) 

addSceneObject(myPlane)
sourceObject:setLuaPath("lua\\sourceObject1.lua");