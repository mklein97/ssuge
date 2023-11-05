local Enemy = {}

Enemy.onStart = function(self, params)
	ssuge.log("got to Enemy.onStart!")
    ssuge.log("my name is " .. self.name .. " and group is " .. self.group)
end

Enemy.onUpdate = function(self, params)
    local player = ssuge.getGameObject("invaderScene", "ObservatoryBase")
    selfx, selfy, selfz = self:getPosition()
    px, py, pz = player:getPosition()
    if selfx > px then
        self:translate(-0.1, 0, 0)
    elseif selfx < px then
        self:translate(0.1, 0, 0)
    end
    if selfy > py then
        self:translate(0, -0.1, 0)
    end
end


return Enemy