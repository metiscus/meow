#!/usr/bin/env python
import wx
import wx.lib.agw.aui as aui
import wx.dataview as dv
import Resource
import os
import xml.etree.ElementTree as ET

RESOURCE_NODE_NAME = "Resources"

RESOURCE_TYPE_MAP = { "ce2f105b-2538-43ea-9eb0-24b1fc1c97cb" : "ImageAtlas" }

#Encapsulate the tree editing functionality
class TreeEdit(wx.Window):
    def __init__(self, parent):
        global RESOURCE_NODE_NAME
        wx.Window.__init__(self, parent)
        self.control = wx.ListCtrl(self, -1, style=wx.LC_REPORT)
        self.control.InsertColumn(0, 'Name')
        self.control.InsertColumn(1, 'GUID')
        self.control.InsertColumn(2, 'Type')
        self.control.InsertColumn(4, 'File')

        # Vertical box sizer for controls
        box = wx.BoxSizer(wx.VERTICAL)
        box.Add(self.control, 1, wx.EXPAND)
        self.SetAutoLayout(True)
        self.SetSizer(box)
        self.Layout()

        self.Show(True)
        self.tasks = {}

    def UpdateResourceTree(self, path):
        for root, dirs, files in os.walk(path):
            for file in files:
                filename, extension = os.path.splitext(file)
                if extension == '.xml':
                    self.InsertResource(root, file)

    def InsertResource(self, path, filename):
        global RESOURCE_TYPE_MAP
        filepath = os.path.join('/', path, filename)
        print filepath
        tree = ET.parse(filepath)
        root = tree.getroot()
        name, extension = os.path.splitext(filepath)
        num_items = self.control.GetItemCount()
        self.control.InsertStringItem(num_items, filename)
        self.control.SetStringItem(num_items, 1, root.get('uuid'))
        typeId = root.get('type')
        if typeId in RESOURCE_TYPE_MAP:
            self.control.SetStringItem(num_items, 2, RESOURCE_TYPE_MAP[typeId])
        else:
            self.control.SetStringItem(num_items, 2, typeId)
        self.control.SetStringItem(num_items, 3, name + extension)
