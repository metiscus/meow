#!/usr/bin/env python
import wx
import MainFrame
import gettext

gettext.install('Editor')

app = wx.App(False)
frame = MainFrame.MainFrame(None, _('Editor'))
app.MainLoop()
