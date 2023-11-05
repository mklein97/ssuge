local Mover = {}
local bList = {}
local eList = {}
local bnum = 0
local enemyNum = 4

Mover.onStart = function(self, params)
	ssuge.log("got to Mover.onStart!")
    ssuge.log("my name is " .. self.name .. " and group is " .. self.group)
    ssuge.setBGM("..\\media\\my_media\\bgm.wav", 0.1)
    ssuge.registerInputListener(self)
    eList[1] = ssuge.getGameObject("invaderScene", "Suzanne")
    eList[2] = ssuge.getGameObject("invaderScene", "Suzanne2")
    eList[3] = ssuge.getGameObject("invaderScene", "Suzanne3")
    eList[4] = ssuge.getGameObject("invaderScene", "Suzanne4")
end

Mover.onUpdate = function(self, params)
    local JOY_DEAD_ZONE = 8000
    local leftstickX = ssuge.getControllerAxisOrTriggerValue(1, "LStickX")
    local leftstickY = ssuge.getControllerAxisOrTriggerValue(1, "LStickY")
    if ssuge.isKeyDown("A") or ssuge.isKeyDown("Left") or ssuge.isControllerButtonDown(1, "Left") or leftstickX < -JOY_DEAD_ZONE then
        self:translate(-0.3, 0, 0)
    elseif ssuge.isKeyDown("D") or ssuge.isKeyDown("Right") or ssuge.isControllerButtonDown(1, "Right") or leftstickX > JOY_DEAD_ZONE then
        self:translate(0.3, 0, 0)
    end
    for i = 0, bnum do
        if bList[i] ~= nil then
            x,y,z = bList[i]:getPosition()
            if y > 80 then
                ssuge.deleteGameObject(bList[i]:getGroupName(), bList[i]:getName())
                bList[i] = nil
            end
            for j = 1, #eList do
                if eList[j] ~= nil then
                    if bList[i]:isColliding(eList[j]) then
                        ssuge.deleteGameObject(bList[i]:getGroupName(), bList[i]:getName())
                        ssuge.deleteGameObject(eList[j]:getGroupName(), eList[j]:getName())
                        bList[i] = nil
                        eList[j] = nil
                    end
                end
            end
        end
    end
    if eList[1] == nil and eList[2] == nil and eList[3] == nil and eList[4] == nil then
        posx = 6
        for i = 1, 4 do
            eList[i] = ssuge.createGameObject("Enemies", "enemy" .. i, "..\\media\\my_media\\enemy.lua")
            elist[i]:createMeshComponent("Suzanne.mesh")
            eList[i]:setPosition(posx, 49.4, 1.1)
            posx = posx - 4
        end
    end
end

Mover.onKeyDown = function(self, params)
    if ssuge.isKeyDown("Space") or ssuge.isControllerButtonDown(1, "A") then
       bList[bnum] = ssuge.createGameObject("Bullets", "bullet" .. bnum, "..\\media\\my_media\\Bullet.lua")
       bList[bnum]:createMeshComponent("Icosphere.mesh")
       bList[bnum]:setScale(0.6, 0.6, 0.6)
       x, y, z = self:getPosition()
       bList[bnum]:setPosition(x, -11, z)
       bnum = bnum + 1
    end
  
end

return Mover
