require("bot")
require("utils")

local bot = createBot()
local w = 8
local h = 4
local flip = false
product = "minecraft:birch_log"
seed = "minecraft:birch_sapling"

local fw = function(c)
    for n = 1, c do
        turtle.suck()
        bot.refuel()
        bot.forward()
        turtle.suck()
    end
end 

local plant = function(seed)
    for n = 1, 16 do
        if turtle.getItemCount(n) > 0 then
            local detail = turtle.getItemDetail(n)
            if detail.name == seed then
                turtle.select(n)
                turtle.place()
                return true
            end
        end
    end
    return false
end

local harvest2 = function()
    local a, b = turtle.inspect()
    if a and b.name == product then
        turtle.dig()
        bot.refuel()
        bot.forward()

        while true do
            a, b = turtle.inspectUp()
            if a and b.name == product then
                turtle.digUp()
                bot.refuel()
                bot.up()

                for n = 1, 4 do
                    turtle.dig()
                    turtle.turnLeft()
                end

            else
                break
            end
        end

        while true do
            a, b = turtle.inspectDown()
            if a == false then
                bot.refuel()
                bot.down()
            else
                break
            end
        end

        bot.refuel()
        bot.back()

        plant(seed)

    end
end

local harvest = function()
    fw(1)

    bot.turnRight()
    turtle.suck()
    harvest2()

    bot.turnLeft()
    bot.turnLeft()
    turtle.suck()

    harvest2()
    bot.turnRight()
end

while true do
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
            fw(4)
            bot.turnRight()
        else
            bot.turnLeft()
            fw(4)
            bot.turnLeft()
        end
        flip = not flip
    end

    bot.gohome()
    bot.refuel()
    bot.back()

    bot.refuel()
    bot.forward()
    sleep(300)
end