#!/usr/bin/env python
import wx
class Resource:
    sId = 0;

    def __init__(self, id=++sId, name="", guid="", typename=""):
        self.id = id
        self.name = name
        self.guid = guid
        self.type = typename
