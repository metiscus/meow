#!/usr/bin/env python
import wx
import wx.lib.agw.aui as aui
import wx.dataview as dv
import TreeEdit
import DetailEdit
import os

class MainFrame(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title=title, size=(600,500))
        self.LayoutForm()

        # Set up the resource root
        self.resource_root = os.getenv("RESOURCE_PATH");
        if self.resource_root == None:
            self.resource_root = os.getcwd()

        # look for resources
        self.tree.UpdateResourceTree(self.resource_root)

    def LayoutForm(self):
        splitter = wx.SplitterWindow(self, -1, style=wx.SP_LIVE_UPDATE)
        splitter.SetMinimumPaneSize(400)
        tree = TreeEdit.TreeEdit(splitter)
        self.tree = tree
        self.details = DetailEdit.DetailEdit(splitter)
        splitter.SplitVertically(self.tree, self.details)
        self.control = splitter

        filemenu = wx.Menu()
        filemenu.Append(wx.ID_ABOUT, "&About", "About this program")
        m_exit = filemenu.Append(wx.ID_EXIT, "E&xit", "Exit the program");
        self.Bind(wx.EVT_MENU, self.OnExit, m_exit)
        menubar = wx.MenuBar()
        menubar.Append(filemenu, "&File")

        self.SetMenuBar(menubar)
        self.Show(True)

    def OnExit(self, event):
        exit()
