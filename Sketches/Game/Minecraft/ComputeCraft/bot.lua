function createBot()
    local o = {}
    o.pos = vector.new(0, 0, 0)
    o.dir = 0
    o.fuels = { "minecraft:stick", "minecraft:charcoal", "minecraft:coal" }
    o.autoFuel = true
    
    local directions = {}
    directions[0] = vector.new(0, 0, -1)
    directions[1] = vector.new(-1, 0, 0)
    directions[2] = vector.new(0, 0, 1)
    directions[3] = vector.new(1, 0, 0)

    local fuelConsumptionType = 0
    
    o.forward = function()
        if o.autoFuel and turtle.getFuelLevel() == 0 then o.refuel() end
    	if turtle.forward() then
    		o.pos = o.pos + directions[o.dir]
    		return true
    	end
    	return false
    end
    
    o.back = function()
        if o.autoFuel and turtle.getFuelLevel() == 0 then o.refuel() end
    	if turtle.back() then
    		o.pos = o.pos + directions[((o.dir - 2) % 4)]
    		return true
    	end
    	return false
    end
    
    o.up = function()
        if o.autoFuel and turtle.getFuelLevel() == 0 then o.refuel() end
    	if turtle.up() then
    	    o.pos = o.pos + vector.new(0, 1, 0)
    		return true
    	end
    	return false
    end
    
    o.down = function()
        if o.autoFuel and turtle.getFuelLevel() == 0 then o.refuel() end
    	if turtle.down() then
    	    o.pos = o.pos + vector.new(0, -1, 0)
    		return true
    	end
    	return false
    end
    
    o.turnLeft = function()
        if turtle.turnLeft() then 
            o.dir = (o.dir + 1) % 4
            return true
        end
        return false
    end
    
    o.turnRight = function()
        if turtle.turnRight() then 
            o.dir = (o.dir - 1) % 4
            return true
        end
        return false
    end

    o.headingTo =  function(headingTo)
        while true do
            local d = headingTo - o.dir

            if d == 0 then
                break
            elseif (d <= -3 or d > 0) or d == -3 then
                o.turnLeft()
            elseif (d > -3 and d < 0) or d == 3 then
                o.turnRight()
            end
        end
    end

    o.moveTo = function(x, y, z)

        local dy = y - o.pos.z
        if dy ~= 0 then
            if dy < 0 then
                o.headingTo(0)
            else
                o.headingTo(2)
            end

            dy = math.abs(dy)
            while dy > 0 do
                o.refuel()
                o.forward()
                dy = math.abs(y - o.pos.z)
            end
        end

        local dx = x - o.pos.x
        if dx ~= 0 then
            if dx < 0 then
                o.headingTo(1)
            else
                o.headingTo(3)
            end

            dx = math.abs(dx)
            while dx > 0 do
                o.refuel()
                o.forward()
                dx = math.abs(x - o.pos.x)
            end
        end
    end

    o.refuel = function()
        local fuelLevel = turtle.getFuelLevel()
        if fuelLevel == "unlimited" or fuelLevel > 0 then
            return
        end
    
        local function tryRefuelDefault()
            for n = 1, 16 do
                if turtle.getItemCount(n) > 0 then
                    turtle.select(n)
                    if turtle.refuel(1) then
                        turtle.select(1)
                        return true
                    end
                end
            end
            turtle.select(1)
            return false
        end
    
        local function tryRefuelPriority()
            for i, value in ipairs(o.fuels) do
                for n = 1, 16 do
                    if turtle.getItemCount(n) > 0 then
                        local item = turtle.getItemDetail(n)
                        if item.name == value then
                            turtle.select(n)
                            if turtle.refuel(1) then
                                turtle.select(1)
                                return true
                            end
                        end
                    end
                end
            end
            turtle.select(1)
            return false
        end
    
        if fuelConsumptionType == 1 then
            if not tryRefuelPriority() then
                print("Add more fuel to continue.")
                while not tryRefuelPriority() do
                    os.pullEvent("turtle_inventory")
                end
                print("Resuming.")
            end
        else
            if not tryRefuelDefault() then
                print("Add more fuel to continue.")
                while not tryRefuelDefault() do
                    os.pullEvent("turtle_inventory")
                end
                print("Resuming.")
            end
        end
    end

    o.gohome = function()
        o.moveTo(0, 0, 0)
        o.headingTo(0)
    end

    o.print = function()
        print(o.pos, o.dir)
    end

    o.hasItem = function(name)
        for n = 1, 16 do
            if turtle.getItemCount(n) > 0 then
                local detail = turtle.getItemDetail(n)
                if detail.name == name then
                    return true
                end
            end
        end
        return false
    end

    o.getItem = function(name)
        for n = 1, 16 do
            if turtle.getItemCount(n) > 0 then
                local detail = turtle.getItemDetail(n)
                if detail.name == name then
                    return detail
                end
            end
        end
        return nil
    end

    o.placeItem = function(name)
        for n = 1, 16 do
            if turtle.getItemCount(n) > 0 then
                local detail = turtle.getItemDetail(n)
                if detail.name == name then
                    turtle.select(n)
                    local a, b = turtle.place()
                    return a
                end
            end
        end
        return false
    end

    return o
end