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
     "text": "ambitap.energyvec~",
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
     "text": "Active-intensity (energy vector) DOA estimate. Uses W/Y/Z/X; outputs x/y/z."
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
     "text": "ambitap.encode~ 1",
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
     "numoutlets": 3,
     "patching_rect": [
      40.0,
      188.0,
      200.0,
      22.0
     ],
     "text": "ambitap.energyvec~",
     "outlettype": [
      "signal",
      "signal",
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "number~",
     "numinlets": 2,
     "numoutlets": 2,
     "patching_rect": [
      40.0,
      244.0,
      80.0,
      22.0
     ],
     "outlettype": [
      "signal",
      "float"
     ],
     "mode": 2
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      270.0,
      80.0,
      18.0
     ],
     "text": "x (front+)"
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "number~",
     "numinlets": 2,
     "numoutlets": 2,
     "patching_rect": [
      130.0,
      244.0,
      80.0,
      22.0
     ],
     "outlettype": [
      "signal",
      "float"
     ],
     "mode": 2
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      130.0,
      270.0,
      80.0,
      18.0
     ],
     "text": "y (left+)"
    }
   },
   {
    "box": {
     "id": "obj-11",
     "maxclass": "number~",
     "numinlets": 2,
     "numoutlets": 2,
     "patching_rect": [
      220.0,
      244.0,
      80.0,
      22.0
     ],
     "outlettype": [
      "signal",
      "float"
     ],
     "mode": 2
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      220.0,
      270.0,
      80.0,
      18.0
     ],
     "text": "z (up+)"
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      480.0,
      96.0,
      110.0,
      20.0
     ],
     "text": "smoothing_time"
    }
   },
   {
    "box": {
     "id": "obj-14",
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
     "id": "obj-15",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      546.0,
      114.0,
      90.0,
      22.0
     ],
     "text": "smoothing_time $1",
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
      "obj-9",
      0
     ],
     "source": [
      "obj-6",
      1
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
      "obj-6",
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
      "obj-14",
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
      "obj-15",
      0
     ]
    }
   }
  ]
 }
}