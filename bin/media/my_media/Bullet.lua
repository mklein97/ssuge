local Bullet = {}

Bullet.onStart = function(self, params)
	ssuge.log("got to Bullet.onStart!")
    ssuge.log("my name is " .. self.name .. " and group is " .. self.group)
    --ssuge.registerInputListener(self)
    self:createSoundComponent("..\\media\\my_media\\pew.wav")
end

Bullet.onUpdate = function(self, params)
    self:translate(0, 0.8, 0)
end

return Bullet