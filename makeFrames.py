"""
The MIT License (MIT)

Copyright (c) 2016 Ryan Shuttleworth, rshuttle@gmail.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
"""

import os
import sys
import math

class Bitmap:
    """A simple class to fiddle with beam frames"""

    def __init__(self, h_bytes, v_bytes):
        """
        h_bytes = width/cols in bytes
        v_bytes = num rows of h_bytes
        so Bitmap(3, 5) is a map of 24 x 5 bits/pixels
        """
        self.h_bytes = h_bytes
        self.v_bytes = v_bytes
        self.set(0)

    def set(self, v):
        self.map = [0] * (self.h_bytes * self.v_bytes)

    def getByte(self, h, v):
        return self.map[v * self.h_bytes + h]

    def setByte(self, h, v=0):
        self.map[v * self.h_bytes + h] = v

    def plot(self, x, y, v=True):
        by = ((y % self.v_bytes) * self.h_bytes) + ((x / 8) % self.h_bytes)
        bt = 1 << (7 - (x % 8))
        if v:
            self.map[by] = self.map[by] | bt
        else:
            self.map[by] = self.map[by] & (bt ^ 255)

    def dump(self):
        print('')
        for v in range(self.v_bytes):
            r = []
            for h in range(self.h_bytes):
                r.append(format(self.getByte(h, v), '#010b'))
            print(', '.join(r))


if __name__ == '__main__':
    bm = Bitmap(3, 5)
    f = 1.0
    # make 36 sine wave frames, increasing in freq with each frame
    for frame in range(36):
        for x in range(24):
            t = (f / 24.0) * float(x)
            y = int(2.5 * math.sin(2 * 3.14159 * f * t) + 2.5)
            bm.plot(x, y)
        bm.dump()
        f = f + (1.0/36.0)
        bm.set(0)
