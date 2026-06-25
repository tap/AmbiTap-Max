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
   920.0,
   620.0
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
     "id": "obj-1",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      14.0,
      400.0,
      22.0
     ],
     "text": "ambitap.compress~",
     "fontsize": 16.0
    }
   },
   {
    "box": {
     "id": "obj-2",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      38.0,
      640.0,
      40.0
     ],
     "text": "Spatial-image-preserving compressor for a HOA bus (detector on W, uniform gain)."
    }
   },
   {
    "box": {
     "id": "obj-3",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      96.0,
      60.0,
      22.0
     ],
     "text": "noise~",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-4",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      140.0,
      150.0,
      22.0
     ],
     "text": "ambitap.encode~ 3",
     "outlettype": [
      "multichannelsignal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-5",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      200.0,
      142.0,
      120.0,
      20.0
     ],
     "text": "(HOA source)"
    }
   },
   {
    "box": {
     "id": "obj-6",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      188.0,
      200.0,
      22.0
     ],
     "text": "ambitap.compress~ 3",
     "outlettype": [
      "multichannelsignal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      244.0,
      180.0,
      120.0
     ],
     "text": "mc.scope~",
     "outlettype": []
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      230.0,
      248.0,
      220.0,
      20.0
     ],
     "text": "all (order+1)^2 channels"
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      96.0,
      110.0,
      20.0
     ],
     "text": "threshold"
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      114.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-11",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      114.0,
      90.0,
      22.0
     ],
     "text": "threshold $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      146.0,
      110.0,
      20.0
     ],
     "text": "ratio"
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      164.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-14",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      164.0,
      90.0,
      22.0
     ],
     "text": "ratio $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-15",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      196.0,
      110.0,
      20.0
     ],
     "text": "attack"
    }
   },
   {
    "box": {
     "id": "obj-16",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      214.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      214.0,
      90.0,
      22.0
     ],
     "text": "attack $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-18",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      246.0,
      110.0,
      20.0
     ],
     "text": "release"
    }
   },
   {
    "box": {
     "id": "obj-19",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      264.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-20",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      264.0,
      90.0,
      22.0
     ],
     "text": "release $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-21",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      296.0,
      110.0,
      20.0
     ],
     "text": "makeup_gain"
    }
   },
   {
    "box": {
     "id": "obj-22",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      480.0,
      314.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-23",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      314.0,
      90.0,
      22.0
     ],
     "text": "makeup_gain $1",
     "outlettype": [
      ""
     ]
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
      "obj-6",
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
      "obj-7",
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
      "obj-11",
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
      "obj-6",
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
      "obj-14",
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
      "obj-6",
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
      "obj-17",
      0
     ],
     "source": [
      "obj-16",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-6",
      0
     ],
     "source": [
      "obj-17",
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
      "obj-19",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-6",
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
      "obj-23",
      0
     ],
     "source": [
      "obj-22",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-6",
      0
     ],
     "source": [
      "obj-23",
      0
     ]
    }
   }
  ]
 }
}