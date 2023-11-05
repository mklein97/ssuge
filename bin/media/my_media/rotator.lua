local Rotator = {}

Rotator.onStart = function(self, params)
	self.rot_speed = 90
end

Rotator.onUpdate = function(self, params)
	local dt = tonumber(params.dt)
	self:rotate(dt * self.rot_speed, 0, 1, 0)
end

return Rotator
