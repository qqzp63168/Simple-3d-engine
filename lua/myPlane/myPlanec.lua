t_CI=myPlane:getControlInfo()
f_vx=(t_CI.right*(-0.05))+(t_CI.left*0.05)
f_vz=(t_CI.up*(-0.08))+(t_CI.down*0.08)
f_x=f_x+f_vx
f_z=f_z+f_vz

if t_CI.up==10 then
if testi <= 1 then
testi=testi+1
zidan=createSceneObject()

zidan:setLuaPath("lua\\zidan\\zidan.lua")
zidan:setName("zid")
createShap("model\\myPlane\\Tiger.x",zidan) 
zidan:setLocation(0,0,0)
addSceneObject(zidan)
end
end