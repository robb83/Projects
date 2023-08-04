hash_set = function(dict, key)
    dict[key] = true
end

hash_has = function(dict, key)
    return dict[key] ~= nil
end

contains = function(list, value)
    for i, v in ipairs(list) do
        if v == value then
            return true
        end
    end
    return false
end