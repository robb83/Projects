const std = @import("std");

pub const DnaError = error {
    EmptyDnaStrands,
    UnequalDnaStrands
};

pub fn compute(first: []const u8, second: []const u8) DnaError!usize {
    
    const flen = first.len;
    const slen = second.len;

    if (flen == 0) {
        return DnaError.EmptyDnaStrands;
    }

    if (slen == 0) {
        return DnaError.EmptyDnaStrands;
    }

    if (flen == slen) {
        var distance: usize = 0;
        var i: usize = 0;
        while (i < flen) {
            if (first[i] != second[i]) {
                distance = distance + 1;
            }
            i = i + 1;
        }

        return distance;
    }

    return DnaError.UnequalDnaStrands;
}
