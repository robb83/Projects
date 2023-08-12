require("bot")
require("utils")

local bot = createBot()
local w = 9
local h = 8
local flip = false
product = "minecraft:wheat"
seed = "minecraft:wheat_seeds"

local harvest = function()
    bot.forward()
    local a, b = turtle.inspectDown()
    if a and b.name == product and b.state.age == 7 then
        turtle.digDown()
        bot.placeItem(seed)
    end
end

while true do
    bot.forward()

    flip = h >= 0
    for nn = 1, h do

        for nnn = 1, w do
            harvest()
        end

        if nn == h then
            break
        end

        if flip then
            bot.turnRight()
            harvest()
            bot.turnRight()
        else
            bot.turnLeft()
            harvest()
            bot.turnLeft()
        end
        flip = not flip
    end

    bot.gohome()
    bot.back()

    local chest = peripheral.wrap("bottom")
    if chest then
        for n = 1, 16 do
            if turtle.getItemCount(n) > 0 then
                turtle.select(n)
                if turtle.refuel(0) == false then
                    turtle.dropDown()
                end
            end
        end
    end

    bot.forward()
    sleep(300)
end