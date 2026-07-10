{
 "patcher": {
  "fileversion": 1,
  "appversion": {
   "major": 8,
   "minor": 0,
   "revision": 0,
   "architecture": "x64",
   "modernui": 1
  },
  "classnamespace": "box",
  "rect": [
   60.0,
   80.0,
   980.0,
   700.0
  ],
  "bglocked": 0,
  "openinpresentation": 0,
  "default_fontsize": 12.0,
  "default_fontface": 0,
  "default_fontname": "Arial",
  "gridonopen": 1,
  "gridsize": [
   15.0,
   15.0
  ],
  "gridsnaponopen": 1,
  "objectsnaponopen": 1,
  "statusbarvisible": 2,
  "toolbarvisible": 1,
  "boxes": [
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      12.0,
      700.0,
      24.0
     ],
     "text": "Hearing in Three Dimensions \u2014 09 \u00b7 Watching the scene",
     "fontsize": 16.0,
     "id": "obj-1"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      40.0,
      930.0,
      20.0
     ],
     "text": "Two sources on a bus, with the compass (energyvec~) and the map (grid~) reading it. (Book: ch. 14.)",
     "id": "obj-2"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      100.0,
      50.0,
      22.0
     ],
     "text": "pink~",
     "outlettype": [
      "signal"
     ],
     "id": "obj-3"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      136.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.2",
     "outlettype": [
      "signal"
     ],
     "id": "obj-4"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      230.0,
      140.0,
      22.0
     ],
     "text": "ambitap.encode~ 3",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-5"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      120.0,
      196.0,
      170.0,
      22.0
     ],
     "text": "azimuth 0.7, elevation 0.26",
     "outlettype": [
      ""
     ],
     "id": "obj-6"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      240.0,
      100.0,
      80.0,
      22.0
     ],
     "text": "cycle~ 330",
     "outlettype": [
      "signal"
     ],
     "id": "obj-7"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      240.0,
      136.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.08",
     "outlettype": [
      "signal"
     ],
     "id": "obj-8"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      240.0,
      230.0,
      140.0,
      22.0
     ],
     "text": "ambitap.encode~ 3",
     "outlettype": [
      "multichannelsignal"
     ],
     "id": "obj-9"
    }
   },
   {
    "box": {
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      320.0,
      196.0,
      100.0,
      22.0
     ],
     "text": "azimuth -2.1",
     "outlettype": [
      ""
     ],
     "id": "obj-10"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 3,
     "patching_rect": [
      430.0,
      330.0,
      140.0,
      22.0
     ],
     "text": "ambitap.energyvec~",
     "outlettype": [
      "signal",
      "signal",
      "signal"
     ],
     "id": "obj-11"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      430.0,
      380.0,
      90.0,
      22.0
     ],
     "text": "snapshot~ 100",
     "outlettype": [
      "float"
     ],
     "id": "obj-12"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      530.0,
      380.0,
      90.0,
      22.0
     ],
     "text": "snapshot~ 100",
     "outlettype": [
      "float"
     ],
     "id": "obj-13"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      630.0,
      380.0,
      90.0,
      22.0
     ],
     "text": "snapshot~ 100",
     "outlettype": [
      "float"
     ],
     "id": "obj-14"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      430.0,
      416.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-15"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      530.0,
      416.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-16"
    }
   },
   {
    "box": {
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      630.0,
      416.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ],
     "id": "obj-17"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      430.0,
      444.0,
      300.0,
      20.0
     ],
     "text": "energy vector: x (front)  y (left)  z (up)",
     "id": "obj-18"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      430.0,
      500.0,
      110.0,
      22.0
     ],
     "text": "ambitap.grid~ 3",
     "outlettype": [
      "multichannelsignal",
      ""
     ],
     "id": "obj-19"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      560.0,
      470.0,
      80.0,
      22.0
     ],
     "text": "qmetro 100",
     "outlettype": [
      "bang"
     ],
     "id": "obj-20"
    }
   },
   {
    "box": {
     "maxclass": "toggle",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      560.0,
      440.0,
      24.0,
      24.0
     ],
     "outlettype": [
      "int"
     ],
     "id": "obj-21"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      430.0,
      540.0,
      70.0,
      22.0
     ],
     "text": "print grid",
     "outlettype": [],
     "id": "obj-22"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      510.0,
      543.0,
      420.0,
      20.0
     ],
     "text": "grid <rows> <cols> <peak_db> <values...> \u2014 the heatmap widget's feed",
     "id": "obj-23"
    }
   },
   {
    "box": {
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      40.0,
      380.0,
      145.0,
      22.0
     ],
     "text": "ambitap.binaural~ 3",
     "outlettype": [
      "signal",
      "signal"
     ],
     "id": "obj-24"
    }
   },
   {
    "box": {
     "maxclass": "ezdac~",
     "numinlets": 2,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      460.0,
      45.0,
      45.0
     ],
     "id": "obj-25"
    }
   },
   {
    "box": {
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      590.0,
      920.0,
      33.0
     ],
     "text": "Mute source 2's *~ and watch y drift positive (left-heavy). Full cockpit with the v8ui widgets: patchers/ambitap.ui-tour.maxpat.",
     "id": "obj-26"
    }
   }
  ],
  "lines": [
   {
    "patchline": {
     "destination": [
      "obj-4",
      0
     ],
     "source": [
      "obj-3",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-4",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-6",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-8",
      0
     ],
     "source": [
      "obj-7",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-9",
      0
     ],
     "source": [
      "obj-8",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-9",
      0
     ],
     "source": [
      "obj-10",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-24",
      0
     ],
     "source": [
      "obj-5",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-11",
      0
     ],
     "source": [
      "obj-5",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-19",
      0
     ],
     "source": [
      "obj-5",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-24",
      0
     ],
     "source": [
      "obj-9",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-11",
      0
     ],
     "source": [
      "obj-9",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-19",
      0
     ],
     "source": [
      "obj-9",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-12",
      0
     ],
     "source": [
      "obj-11",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-13",
      0
     ],
     "source": [
      "obj-11",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-14",
      0
     ],
     "source": [
      "obj-11",
      2
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-15",
      0
     ],
     "source": [
      "obj-12",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-16",
      0
     ],
     "source": [
      "obj-13",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-17",
      0
     ],
     "source": [
      "obj-14",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-20",
      0
     ],
     "source": [
      "obj-21",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-19",
      0
     ],
     "source": [
      "obj-20",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-22",
      0
     ],
     "source": [
      "obj-19",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-25",
      0
     ],
     "source": [
      "obj-24",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-25",
      1
     ],
     "source": [
      "obj-24",
      1
     ]
    }
   }
  ]
 }
}
