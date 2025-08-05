MissionSystem = GetMissionManager() 
MissionSystem:ClearAll() 
 
MissionSystem:AddSubMission{ 
	 submissionid = 100,
	 completecondition = {
		 mode = -1,
		 stagerange = { -1, -1 }, 
		 itemid = 25970,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25970,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 6,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25630,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 25630,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 8,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25670,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25670,
		 itemnum = 1,
		 probability = 0.250000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [11] = 1,  },
		 mapid = {8,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25710,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25710,
		 itemnum = 1,
		 probability = 0.350000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [10] = 1,  },
		 mapid = {8,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25750,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25750,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = 220,
		 stagerange = { 25, 25},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25830,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25830,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 1, 5},
		 mode = {15,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25990,
		 itemnum = 15,
	}, 
	 dropinfo = {
		 itemid = 25990,
		 itemnum = 1,
		 probability = 0.400000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 800,
	 completecondition = {
		 mode = 15,
		 stagerange = { 5, 5 }, 
		 itemid = -1,
		 itemnum = -1,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = -1,
		 probability = 0.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 8880,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = -1,
		 probability = 0.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26690,
		 itemnum = 300,
	}, 
	 dropinfo = {
		 itemid = 26690,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26690,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 26690,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26690,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 26690,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26700,
		 itemnum = 300,
	}, 
	 dropinfo = {
		 itemid = 26700,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26700,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 26700,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26700,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 26700,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26710,
		 itemnum = 300,
	}, 
	 dropinfo = {
		 itemid = 26710,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26710,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 26710,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26710,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 26710,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 1900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26720,
		 itemnum = 300,
	}, 
	 dropinfo = {
		 itemid = 26720,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 2000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26720,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 26720,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 2100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26720,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 26720,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 2200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26730,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 26730,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 2400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26740,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 26740,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 2600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26750,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 26750,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 2800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26760,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 26760,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26770,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 26770,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {26,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26780,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 26780,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {26,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 26790,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 26790,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {26,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 8880,
		 itemnum = 2,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = -1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 2300,
		 itemnum = -1,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = -1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25680,
		 itemnum = 3,
	}, 
	 dropinfo = {
		 itemid = 25680,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = 21,
		 stagerange = { -1, -1},
		 mode = {4,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25640,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 25640,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 8,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28500,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 28500,
		 itemnum = 1,
		 probability = 0.270000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6, 15,  },
		 monsters = { [11] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25730,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25730,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = 220,
		 stagerange = { 25, 25},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 3900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25700,
		 itemnum = 5,
	}, 
	 dropinfo = {
		 itemid = 25700,
		 itemnum = 1,
		 probability = 0.800000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [2] = 5,  },
		 mapid = {3,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28490,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 28490,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {13,  },
		 monsters = { [23] = 1,  [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25790,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 25790,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25800,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 25800,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28550,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 28550,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28560,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 28560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25790,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25790,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25800,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25800,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28550,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 28550,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28560,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 28560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { [25] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 4900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28810,
		 itemnum = 5,
	}, 
	 dropinfo = {
		 itemid = 28810,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = 15,
		 stagerange = { -1, -1},
		 mode = {4,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29400,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29400,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 1, 1},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29410,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29410,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 2, 2},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29420,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29420,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 3, 3},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29430,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29430,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 4, 4},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29440,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29440,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 5, 5},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29450,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29450,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 6, 6},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29460,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29460,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 7, 7},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29470,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29470,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 8, 8},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 5900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29480,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29480,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 9, 9},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29490,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29490,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 10, 10},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29500,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29500,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 11, 11},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29510,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29510,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 12, 12},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29520,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29520,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 13, 13},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29530,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29530,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 14, 14},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29540,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29540,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 15, 15},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29550,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29550,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 16, 16},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29560,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 17, 17},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29570,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29570,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 18, 18},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 6900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29580,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29580,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 19, 19},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29590,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29590,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 20, 20},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29600,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29600,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 21, 21},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29610,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29610,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { 22, 22},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29620,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29620,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 23, 23},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29630,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29630,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 24, 24},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29950,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29950,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {27,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29960,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29960,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29970,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29970,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29980,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29980,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {12,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 7900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29990,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29990,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {13,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25650,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 25650,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 8,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 12520,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 12520,
		 itemnum = 1,
		 probability = 0.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 30300,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 30300,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { 25, 25},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25720,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25720,
		 itemnum = 1,
		 probability = 0.300000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [10] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 30310,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 30310,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {13,  },
		 monsters = { [0] = 1,  [1] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8401,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 30310,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 30310,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {13,  },
		 monsters = { [32] = 1,  [33] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25850,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25850,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 1, 5},
		 mode = {15,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29890,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 29890,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {27,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29880,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29880,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29880,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 29880,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 8900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29900,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29900,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29900,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 29900,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29910,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29910,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {13,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25670,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25670,
		 itemnum = 1,
		 probability = 0.350000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [11] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25660,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 25660,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 8,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31400,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 31400,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 25, 25},
		 mode = {7,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31410,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 31410,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 5, 5},
		 mode = {15,  },
		 monsters = { [35] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31420,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 31420,
		 itemnum = 1,
		 probability = 0.250000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [10] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31560,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 31560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31560,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 31560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 9900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31560,
		 itemnum = 200,
	}, 
	 dropinfo = {
		 itemid = 31560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31560,
		 itemnum = 400,
	}, 
	 dropinfo = {
		 itemid = 31560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31560,
		 itemnum = 800,
	}, 
	 dropinfo = {
		 itemid = 31560,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31570,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 31570,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31570,
		 itemnum = 300,
	}, 
	 dropinfo = {
		 itemid = 31570,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31570,
		 itemnum = 500,
	}, 
	 dropinfo = {
		 itemid = 31570,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31570,
		 itemnum = 1000,
	}, 
	 dropinfo = {
		 itemid = 31570,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31580,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 31580,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31580,
		 itemnum = 200,
	}, 
	 dropinfo = {
		 itemid = 31580,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31600,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 31600,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = {11,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 10900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31590,
		 itemnum = 100,
	}, 
	 dropinfo = {
		 itemid = 31590,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = 60000,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 31590,
		 itemnum = 200,
	}, 
	 dropinfo = {
		 itemid = 31590,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = 60000,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 32150,
		 itemnum = 20,
	}, 
	 dropinfo = {
		 itemid = 32150,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {28,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 32160,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 32160,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { [53] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 32160,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 32160,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { [53] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25670,
		 itemnum = 3,
	}, 
	 dropinfo = {
		 itemid = 25670,
		 itemnum = 1,
		 probability = 0.100000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { [11] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 32990,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 32990,
		 itemnum = 1,
		 probability = 0.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [58] = 1,  },
		 mapid = {47,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 33860,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 33860,
		 itemnum = 1,
		 probability = 0.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {13,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 11900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37370,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37370,
		 itemnum = 1,
		 probability = 0.300000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [0] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37380,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37380,
		 itemnum = 1,
		 probability = 0.300000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [1] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37390,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37390,
		 itemnum = 1,
		 probability = 0.300000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [22] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37400,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37400,
		 itemnum = 1,
		 probability = 0.200000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [64] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37410,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37410,
		 itemnum = 1,
		 probability = 0.200000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [65] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37420,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37420,
		 itemnum = 1,
		 probability = 0.200000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [63] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37430,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37430,
		 itemnum = 1,
		 probability = 0.200000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [66] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37640,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37640,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {30,  },
		 monsters = { [62] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 37650,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 37650,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {30,  },
		 monsters = { [62] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 36160,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 36160,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { [21] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 12900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 38690,
		 itemnum = 30,
	}, 
	 dropinfo = {
		 itemid = 38690,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {10, 11, 12, 13, 14, 15, 26, 30, 31,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39500,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39500,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [53] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13100,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39490,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 39490,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {12,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13200,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39490,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39490,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {12,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13300,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39510,
		 itemnum = 2,
	}, 
	 dropinfo = {
		 itemid = 39510,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [66] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13400,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39510,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39510,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { [66] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13500,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39520,
		 itemnum = 300,
	}, 
	 dropinfo = {
		 itemid = 39520,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [7] = 1,  },
		 mapid = {6,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13600,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39520,
		 itemnum = 50,
	}, 
	 dropinfo = {
		 itemid = 39520,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [7] = 1,  },
		 mapid = {6,  },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13700,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39710,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39710,
		 itemnum = 1,
		 probability = 0.100000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 27, 30, 31,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39720,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39720,
		 itemnum = 1,
		 probability = 0.100000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 27, 30, 31,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 13900,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39730,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39730,
		 itemnum = 1,
		 probability = 0.100000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 27, 30, 31,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 14000,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 39740,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 39740,
		 itemnum = 1,
		 probability = 0.100000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 27, 30, 31,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58710,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 40820,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 40820,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {6,  },
		 monsters = { [2] = 1,  },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58720,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 40840,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 40840,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = 0,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {31,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58730,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 40830,
		 itemnum = 5,
	}, 
	 dropinfo = {
		 itemid = 40830,
		 itemnum = 1,
		 probability = 0.200000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58750,
	 completecondition = {
		 mode = 7,
		 stagerange = { 1, 1 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58760,
	 completecondition = {
		 mode = 27,
		 stagerange = { 1, 1 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58761,
	 completecondition = {
		 mode = 27,
		 stagerange = { 3, 3 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58762,
	 completecondition = {
		 mode = 10,
		 stagerange = { 1, 1 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58763,
	 completecondition = {
		 mode = 10,
		 stagerange = { 3, 3 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58764,
	 completecondition = {
		 mode = 10,
		 stagerange = { 5, 5 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58765,
	 completecondition = {
		 mode = 11,
		 stagerange = { 1, 1 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58766,
	 completecondition = {
		 mode = 11,
		 stagerange = { 2, 2 }, 
		 itemid = -1,
		 itemnum = 0,
	}, 
	 dropinfo = {
		 itemid = -1,
		 itemnum = 0,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = false,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = { },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58770,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25630,
		 itemnum = 15,
	}, 
	 dropinfo = {
		 itemid = 25630,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58771,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25640,
		 itemnum = 15,
	}, 
	 dropinfo = {
		 itemid = 25640,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58772,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25650,
		 itemnum = 15,
	}, 
	 dropinfo = {
		 itemid = 25650,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58773,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25660,
		 itemnum = 15,
	}, 
	 dropinfo = {
		 itemid = 25660,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58774,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 44660,
		 itemnum = 15,
	}, 
	 dropinfo = {
		 itemid = 44660,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58775,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25630,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 25630,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58776,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25640,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 25640,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58777,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25650,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 25650,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58778,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25660,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 25660,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58779,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 44660,
		 itemnum = 10,
	}, 
	 dropinfo = {
		 itemid = 44660,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {0, 1,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58780,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29880,
		 itemnum = 3,
	}, 
	 dropinfo = {
		 itemid = 29880,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 3, 5},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58781,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29880,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29880,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 3, 5},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58790,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29900,
		 itemnum = 3,
	}, 
	 dropinfo = {
		 itemid = 29900,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58800,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 29890,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 29890,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {27,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58810,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 25990,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 25990,
		 itemnum = 1,
		 probability = 1.000000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 3, 5},
		 mode = {27,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58820,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 28610,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 28610,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58830,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 45090,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 45090,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 5, 5},
		 mode = {10,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58840,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 45110,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 45110,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { -1, -1},
		 mode = {11,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddSubMission{ 
	 submissionid = 58841,
	 completecondition = {
		 mode = -1,
		 stagerange = { 0, 0 }, 
		 itemid = 45100,
		 itemnum = 1,
	}, 
	 dropinfo = {
		 itemid = 45100,
		 itemnum = 1,
		 probability = 0.500000,
		 killnum = -1,
		 die = -1,
		 playtime = -1,
		 bwin = true,
		 score = -1,
		 stagerange = { 2, 5},
		 mode = {11,  },
		 monsters = { },
		 mapid = { },
	},
}

MissionSystem:AddMission{ 
	 missionid = 25610,
	 destmissionid = 25610,
	 precondition = -1,
	 character = 0,
	 promotion = 1,
	 MinLevel = 20,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 100, 200, 300, 400, 600,  },
	 item = {
		 { 25880, -1, -1 },
		 { 25960, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 25620,
	 destmissionid = 25610,
	 precondition = -1,
	 character = 0,
	 promotion = 1,
	 MinLevel = 20,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 200, 300,  },
	 item = {
		 { 25880, -1, -1 },
		 { 25960, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 25980,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 700,  },
	 item = {
		 { 14130, 1, -1 },
		 { 19360, 3, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26830,
	 destmissionid = 26830,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 800,  },
	 item = {
		 { 42410, 3, -1 },
	 },
	 gp = 1000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = 3,
}

MissionSystem:AddMission{ 
	 missionid = 26840,
	 destmissionid = 26830,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 900,  },
	 item = {
		 { 42410, 3, -1 },
	 },
	 gp = 1000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = 3,
}

MissionSystem:AddMission{ 
	 missionid = 26850,
	 destmissionid = 26850,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1000, 4100,  },
	 item = {
		 { 2430, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26860,
	 destmissionid = 26850,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1100, 4500,  },
	 item = {
		 { 2430, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26930,
	 destmissionid = 26850,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1200, 3400,  },
	 item = {
		 { 2430, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26870,
	 destmissionid = 26870,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1300, 4200,  },
	 item = {
		 { 2960, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26880,
	 destmissionid = 26870,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1400, 4600,  },
	 item = {
		 { 2960, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26940,
	 destmissionid = 26870,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1500, 3400,  },
	 item = {
		 { 2960, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26890,
	 destmissionid = 26890,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1600, 4300,  },
	 item = {
		 { 3090, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26900,
	 destmissionid = 26890,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1700, 4700,  },
	 item = {
		 { 3090, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26950,
	 destmissionid = 26890,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1800, 3400,  },
	 item = {
		 { 3090, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26910,
	 destmissionid = 26910,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 1900, 4400,  },
	 item = {
		 { 21320, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26920,
	 destmissionid = 26910,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 2000, 4800,  },
	 item = {
		 { 21320, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26960,
	 destmissionid = 26910,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 2100, 3400,  },
	 item = {
		 { 21320, -1, -1 },
	 },
	 gp = 500,
	 exp = 10,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26800,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 3000,  },
	 item = {
		 { 1580, -1, -1 },
		 { 29080, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26810,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 3100,  },
	 item = {
		 { 1640, -1, -1 },
		 { 29090, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 26820,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 3200,  },
	 item = {
		 { 1700, -1, -1 },
		 { 29100, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 28290,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 30,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 3300,  },
	 item = {
		 { 27760, 30, -1 },
		 { 31430, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 28510,
	 destmissionid = 28510,
	 precondition = -1,
	 character = 1,
	 promotion = 1,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 3500, 3600, 3700, 3900, 4000,  },
	 item = {
		 { 28530, -1, -1 },
		 { 28540, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 28520,
	 destmissionid = 28510,
	 precondition = -1,
	 character = 1,
	 promotion = 1,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 3600, 3900,  },
	 item = {
		 { 28530, -1, -1 },
		 { 28540, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 28800,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 4900,  },
	 item = {
		 { 13700, 1, -1 },
		 { 13710, 1, -1 },
		 { 13720, 1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 28990,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 4900,  },
	 item = {
		 { 13700, 7, -1 },
		 { 13710, 7, -1 },
		 { 13720, 7, -1 },
	 },
	 gp = 10000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29080,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 6350, 3, -1 },
		 { 29120, -1, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44740,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 6350, 3, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29090,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 6410, 3, -1 },
		 { 29130, -1, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44750,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 6410, 3, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29100,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 6470, 3, -1 },
		 { 29140, -1, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44760,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 6470, 3, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29110,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 26040, 3, -1 },
		 { 29150, -1, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44770,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 26040, 3, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29120,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 29160, -1, -1 },
		 { 38450, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44790,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 38450, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29130,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 29170, -1, -1 },
		 { 38460, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44800,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 38460, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29140,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 29180, -1, -1 },
		 { 38470, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44810,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 38470, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29150,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 29190, -1, -1 },
		 { 38480, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44820,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 38480, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29160,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900, 7500,  },
	 item = {
		 { 5400, -1, -1 },
		 { 29200, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44840,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900,  },
	 item = {
		 { 5400, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29170,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900, 7500,  },
	 item = {
		 { 5410, -1, -1 },
		 { 29210, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44850,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900,  },
	 item = {
		 { 5410, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29180,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900, 7500,  },
	 item = {
		 { 5420, -1, -1 },
		 { 29220, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44860,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900,  },
	 item = {
		 { 5420, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29190,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900, 7500,  },
	 item = {
		 { 29230, -1, -1 },
		 { 41860, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44870,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900,  },
	 item = {
		 { 41860, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29200,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200, 7600,  },
	 item = {
		 { 14060, 7, -1 },
		 { 29240, -1, -1 },
	 },
	 gp = 400,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44890,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200,  },
	 item = {
		 { 14060, 7, -1 },
	 },
	 gp = 400,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29210,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200, 7600,  },
	 item = {
		 { 14070, 7, -1 },
		 { 29250, -1, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44900,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200,  },
	 item = {
		 { 14070, 7, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29220,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200, 7600,  },
	 item = {
		 { 14080, 7, -1 },
		 { 29260, -1, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44910,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200,  },
	 item = {
		 { 14080, 7, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29230,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200, 7600,  },
	 item = {
		 { 29270, -1, -1 },
		 { 43910, 7, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44920,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200,  },
	 item = {
		 { 43910, 7, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29240,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500, 7700,  },
	 item = {
		 { 9760, -1, -1 },
		 { 29280, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44940,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500,  },
	 item = {
		 { 9760, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29250,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500, 7700,  },
	 item = {
		 { 9860, -1, -1 },
		 { 29290, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44950,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500,  },
	 item = {
		 { 9860, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29260,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500, 7700,  },
	 item = {
		 { 9960, -1, -1 },
		 { 29300, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44960,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500,  },
	 item = {
		 { 9960, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29270,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500, 7700,  },
	 item = {
		 { 21040, -1, -1 },
		 { 29310, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44970,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500,  },
	 item = {
		 { 21040, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29280,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
		 { 29320, -1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44990,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29290,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
		 { 29330, -1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45000,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29300,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
		 { 29340, -1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45010,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29310,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
		 { 29350, -1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45020,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800, 7800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29320,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
		 { 29360, -1, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45040,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29330,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
		 { 29370, -1, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45050,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29340,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
		 { 29380, -1, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45060,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29350,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
		 { 29390, -1, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45070,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100, 7900,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29360,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 7200, 7300, 7400,  },
	 item = {
		 { 1500, 1, -1 },
		 { 8880, 5, -1 },
	 },
	 gp = 0,
	 exp = 300,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29370,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 7200, 7300, 7400,  },
	 item = {
		 { 1510, 1, -1 },
		 { 8880, 5, -1 },
	 },
	 gp = 0,
	 exp = 300,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29380,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 7200, 7300, 7400,  },
	 item = {
		 { 1520, 1, -1 },
		 { 8880, 5, -1 },
	 },
	 gp = 0,
	 exp = 300,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29390,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 7200, 7300, 7400,  },
	 item = {
		 { 8880, 5, -1 },
		 { 19690, 1, -1 },
	 },
	 gp = 0,
	 exp = 300,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 30320,
	 destmissionid = 30320,
	 precondition = -1,
	 character = 2,
	 promotion = 1,
	 MinLevel = 20,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8000, 8100, 8200, 8300, 8401, 8500,  },
	 item = {
		 { 30000, -1, -1 },
		 { 30460, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 30330,
	 destmissionid = 30320,
	 precondition = -1,
	 character = 2,
	 promotion = 1,
	 MinLevel = 20,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8000, 8100, 8500,  },
	 item = {
		 { 30000, -1, -1 },
		 { 30460, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29760,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8600, 8700,  },
	 item = {
		 { 1080, 30, -1 },
		 { 1090, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29770,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8600, 8700,  },
	 item = {
		 { 1140, 30, -1 },
		 { 1150, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29780,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8600, 8700,  },
	 item = {
		 { 1110, 30, -1 },
		 { 1210, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29790,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8600, 8700,  },
	 item = {
		 { 26240, 30, -1 },
		 { 26250, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29800,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8800, 8900,  },
	 item = {
		 { 6810, 30, -1 },
		 { 6820, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29810,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8800, 8900,  },
	 item = {
		 { 6820, 30, -1 },
		 { 6870, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29820,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8800, 8900,  },
	 item = {
		 { 6930, 30, -1 },
		 { 6940, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29830,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 8800, 8900,  },
	 item = {
		 { 19510, 30, -1 },
		 { 19520, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29840,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9000, 9100,  },
	 item = {
		 { 3450, 30, -1 },
		 { 8880, 3, -1 },
		 { 20070, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29850,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9000, 9100,  },
	 item = {
		 { 3550, 30, -1 },
		 { 8880, 3, -1 },
		 { 20080, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29860,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9000, 9100,  },
	 item = {
		 { 3650, 30, -1 },
		 { 8880, 3, -1 },
		 { 20090, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 29870,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9000, 9100,  },
	 item = {
		 { 8880, 4, -1 },
		 { 19530, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31380,
	 destmissionid = 31380,
	 precondition = -1,
	 character = 3,
	 promotion = 1,
	 MinLevel = 20,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9200, 9300, 9400, 9500, 9600,  },
	 item = {
		 { 31150, -1, -1 },
		 { 31900, -1, -1 },
		 { 31910, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31390,
	 destmissionid = 31380,
	 precondition = -1,
	 character = 3,
	 promotion = 1,
	 MinLevel = 20,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9300, 9400,  },
	 item = {
		 { 31150, -1, -1 },
		 { 31900, -1, -1 },
		 { 31910, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27690,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9700,  },
	 item = {
		 { 31610, 30, -1 },
		 { 31620, 30, -1 },
		 { 31630, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27700,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9700,  },
	 item = {
		 { 31650, 30, -1 },
		 { 31660, 30, -1 },
		 { 31670, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27710,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9700,  },
	 item = {
		 { 31640, 30, -1 },
		 { 31680, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27720,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9700,  },
	 item = {
		 { 31690, 30, -1 },
		 { 31700, 30, -1 },
		 { 31710, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27730,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9800,  },
	 item = {
		 { 8090, 30, -1 },
		 { 8120, 30, -1 },
		 { 8140, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27740,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 9900,  },
	 item = {
		 { 29920, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 27750,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10000,  },
	 item = {
		 { 29930, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31470,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10100,  },
	 item = {
		 { 15660, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31480,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10100,  },
	 item = {
		 { 15760, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31490,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10100,  },
	 item = {
		 { 15860, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31500,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10100,  },
	 item = {
		 { 21000, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31430,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10200,  },
	 item = {
		 { 31440, -1, -1 },
	 },
	 gp = 4000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31440,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10300,  },
	 item = {
		 { 31450, -1, -1 },
	 },
	 gp = 8000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31450,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10400,  },
	 item = {
		 { 31460, -1, -1 },
	 },
	 gp = 12000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31460,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10500,  },
	 item = {
	 },
	 gp = 16000,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31510,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10600,  },
	 item = {
		 { 31720, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31520,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10700,  },
	 item = {
		 { 31730, 30, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31530,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10800,  },
	 item = {
		 { 28290, -1, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31540,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 10900,  },
	 item = {
		 { 30790, 7, -1 },
		 { 30800, 7, -1 },
		 { 30810, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 31550,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 11000,  },
	 item = {
		 { 30790, 15, -1 },
		 { 30800, 15, -1 },
		 { 30810, 15, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32190,
	 destmissionid = 32190,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 11100, 11200, 11400,  },
	 item = {
		 { 32250, 100, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = 4,
}

MissionSystem:AddMission{ 
	 missionid = 32170,
	 destmissionid = 32190,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 11300,  },
	 item = {
		 { 32250, 100, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = 4,
}

MissionSystem:AddMission{ 
	 missionid = 32450,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 32460, -1, -1 },
		 { 44710, 3, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44780,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100, 5200, 5300,  },
	 item = {
		 { 44710, 3, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32460,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 32470, -1, -1 },
		 { 38490, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44830,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5400, 5500, 5600,  },
	 item = {
		 { 38490, 3, -1 },
	 },
	 gp = 300,
	 exp = 12,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32470,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900,  },
	 item = {
		 { 32480, -1, -1 },
		 { 41850, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44880,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5700, 5800, 5900,  },
	 item = {
		 { 41850, -1, -1 },
	 },
	 gp = 400,
	 exp = 14,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32480,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200,  },
	 item = {
		 { 32490, -1, -1 },
		 { 43920, 7, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44930,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6000, 6100, 6200,  },
	 item = {
		 { 43920, 7, -1 },
	 },
	 gp = 450,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32490,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500,  },
	 item = {
		 { 32500, -1, -1 },
		 { 41960, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44980,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6300, 6400, 6500,  },
	 item = {
		 { 41960, -1, -1 },
	 },
	 gp = 500,
	 exp = 40,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32500,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
		 { 32510, -1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45030,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6600, 6700, 6800,  },
	 item = {
		 { 8880, 5, -1 },
		 { 26680, 1, -1 },
	 },
	 gp = 0,
	 exp = 80,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32510,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
		 { 32520, -1, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 45080,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 6900, 7000, 7100,  },
	 item = {
		 { 8880, 5, -1 },
		 { 12700, 7, -1 },
	 },
	 gp = 0,
	 exp = 160,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 32520,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 7200, 7300, 7400,  },
	 item = {
		 { 8880, 3, -1 },
		 { 32440, 1, -1 },
	 },
	 gp = 0,
	 exp = 320,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37300,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 11900,  },
	 item = {
		 { 37440, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37310,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12000,  },
	 item = {
		 { 37450, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37320,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12100,  },
	 item = {
		 { 37460, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37330,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12200,  },
	 item = {
		 { 37470, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37340,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12300,  },
	 item = {
		 { 37480, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37350,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12400,  },
	 item = {
		 { 37490, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37360,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12500,  },
	 item = {
		 { 37500, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37600,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12600,  },
	 item = {
		 { 37620, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 37610,
	 destmissionid = 0,
	 precondition = -1,
	 character = -1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12700,  },
	 item = {
		 { 37630, 7, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 36170,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12800,  },
	 item = {
		 { 38550, 3, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 36180,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12800,  },
	 item = {
		 { 38560, 3, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 36190,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12800,  },
	 item = {
		 { 38570, 3, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 36200,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12800,  },
	 item = {
		 { 36100, 3, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 36210,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12800,  },
	 item = {
		 { 36110, 3, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 38640,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12900,  },
	 item = {
	 },
	 gp = 10,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 38650,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12900,  },
	 item = {
	 },
	 gp = 10,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 38660,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12900,  },
	 item = {
	 },
	 gp = 10,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 38670,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12900,  },
	 item = {
	 },
	 gp = 10,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 38680,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 12900,  },
	 item = {
	 },
	 gp = 10,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39000,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13000, 13100,  },
	 item = {
		 { 39620, -1, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39010,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13200,  },
	 item = {
		 { 39630, -1, -1 },
	 },
	 gp = 200,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39620,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13300,  },
	 item = {
		 { 39640, -1, -1 },
	 },
	 gp = 150,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39630,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13400,  },
	 item = {
		 { 39650, -1, -1 },
	 },
	 gp = 150,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39640,
	 destmissionid = 39650,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13500,  },
	 item = {
		 { 39020, -1, -1 },
		 { 39030, -1, -1 },
		 { 39530, 100, -1 },
	 },
	 gp = 150,
	 exp = 5,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39650,
	 destmissionid = 39650,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 5,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13600,  },
	 item = {
		 { 39020, -1, -1 },
		 { 39030, -1, -1 },
		 { 39530, 100, -1 },
	 },
	 gp = 150,
	 exp = 5,
	 bPromotion = 1,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39750,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13700, 13800, 13900, 14000,  },
	 item = {
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39760,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13700, 13800, 13900, 14000,  },
	 item = {
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39770,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13700, 13800, 13900, 14000,  },
	 item = {
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39780,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13700, 13800, 13900, 14000,  },
	 item = {
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 39790,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 13700, 13800, 13900, 14000,  },
	 item = {
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 40850,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58710, 58720, 58730,  },
	 item = {
		 { 33550, 15, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 40860,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58710, 58720, 58730,  },
	 item = {
		 { 33560, 15, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 40870,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58710, 58720, 58730,  },
	 item = {
		 { 33570, 15, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 40880,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58710, 58720, 58730,  },
	 item = {
		 { 33580, 15, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 40890,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58710, 58720, 58730,  },
	 item = {
		 { 40810, 15, -1 },
	 },
	 gp = 0,
	 exp = 0,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44020,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100,  },
	 item = {
		 { 160, -1, -1 },
		 { 220, -1, -1 },
		 { 720, -1, -1 },
	 },
	 gp = 20,
	 exp = 3,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44030,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100,  },
	 item = {
		 { 180, -1, -1 },
		 { 230, -1, -1 },
		 { 290, -1, -1 },
	 },
	 gp = 20,
	 exp = 3,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44040,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100,  },
	 item = {
		 { 200, -1, -1 },
		 { 250, -1, -1 },
		 { 310, -1, -1 },
	 },
	 gp = 20,
	 exp = 3,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44050,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100,  },
	 item = {
		 { 43950, -1, -1 },
		 { 44610, -1, -1 },
		 { 44620, -1, -1 },
	 },
	 gp = 20,
	 exp = 3,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44060,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 5100,  },
	 item = {
		 { 43960, -1, -1 },
		 { 44630, -1, -1 },
		 { 44640, -1, -1 },
	 },
	 gp = 20,
	 exp = 3,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44070,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58800,  },
	 item = {
		 { 2320, 3, -1 },
	 },
	 gp = 50,
	 exp = 4,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44080,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58800,  },
	 item = {
		 { 2320, 3, -1 },
	 },
	 gp = 50,
	 exp = 4,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44090,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58800,  },
	 item = {
		 { 2320, 3, -1 },
	 },
	 gp = 50,
	 exp = 4,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44100,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58800,  },
	 item = {
		 { 2320, 3, -1 },
	 },
	 gp = 50,
	 exp = 4,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44110,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58800,  },
	 item = {
		 { 2320, 3, -1 },
	 },
	 gp = 50,
	 exp = 4,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44170,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58810,  },
	 item = {
		 { 4420, 3, -1 },
	 },
	 gp = 100,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44180,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58810,  },
	 item = {
		 { 4420, 3, -1 },
	 },
	 gp = 100,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44190,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58810,  },
	 item = {
		 { 4420, 3, -1 },
	 },
	 gp = 100,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44200,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58810,  },
	 item = {
		 { 4420, 3, -1 },
	 },
	 gp = 100,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44210,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58810,  },
	 item = {
		 { 4420, 3, -1 },
	 },
	 gp = 100,
	 exp = 5,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44220,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58820,  },
	 item = {
		 { 1540, 3, -1 },
	 },
	 gp = 150,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44230,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58820,  },
	 item = {
		 { 1600, 3, -1 },
	 },
	 gp = 150,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44240,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58820,  },
	 item = {
		 { 1660, 3, -1 },
	 },
	 gp = 150,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44250,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58820,  },
	 item = {
		 { 18640, 3, -1 },
	 },
	 gp = 150,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44260,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58820,  },
	 item = {
		 { 44680, 3, -1 },
	 },
	 gp = 150,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44270,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58781,  },
	 item = {
		 { 19360, 1, -1 },
	 },
	 gp = 250,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44280,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58781,  },
	 item = {
		 { 19360, 1, -1 },
	 },
	 gp = 250,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44290,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58781,  },
	 item = {
		 { 19360, 1, -1 },
	 },
	 gp = 250,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44300,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58781,  },
	 item = {
		 { 19360, 1, -1 },
	 },
	 gp = 250,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44310,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58781,  },
	 item = {
		 { 19360, 1, -1 },
	 },
	 gp = 250,
	 exp = 7,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44320,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58830,  },
	 item = {
		 { 13760, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44330,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58830,  },
	 item = {
		 { 13760, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44340,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58830,  },
	 item = {
		 { 13760, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44350,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58830,  },
	 item = {
		 { 13760, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44360,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58830,  },
	 item = {
		 { 13760, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44470,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58840,  },
	 item = {
		 { 8980, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44480,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58840,  },
	 item = {
		 { 9080, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44490,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58840,  },
	 item = {
		 { 9180, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44500,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58840,  },
	 item = {
		 { 20390, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44510,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58840,  },
	 item = {
		 { 37700, -1, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44520,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58841,  },
	 item = {
		 { 2770, -1, -1 },
	 },
	 gp = 350,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44530,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58841,  },
	 item = {
		 { 2830, -1, -1 },
	 },
	 gp = 350,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44540,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58841,  },
	 item = {
		 { 2890, -1, -1 },
	 },
	 gp = 350,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44550,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58841,  },
	 item = {
		 { 23150, -1, -1 },
	 },
	 gp = 350,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44560,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58841,  },
	 item = {
		 { 33350, -1, -1 },
	 },
	 gp = 350,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44420,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58770,  },
	 item = {
		 { 2910, -1, -1 },
	 },
	 gp = 450,
	 exp = 15,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44430,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58771,  },
	 item = {
		 { 2920, -1, -1 },
	 },
	 gp = 450,
	 exp = 15,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44440,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58772,  },
	 item = {
		 { 2930, -1, -1 },
	 },
	 gp = 450,
	 exp = 15,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44450,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58773,  },
	 item = {
		 { 23160, -1, -1 },
	 },
	 gp = 450,
	 exp = 15,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44460,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58774,  },
	 item = {
		 { 32360, -1, -1 },
		 { 32370, -1, -1 },
	 },
	 gp = 450,
	 exp = 15,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44370,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58775,  },
	 item = {
		 { 8080, 7, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44380,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58776,  },
	 item = {
		 { 8110, 7, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44390,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58777,  },
	 item = {
		 { 8140, 7, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44400,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58778,  },
	 item = {
		 { 43930, 7, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44410,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58779,  },
	 item = {
		 { 43940, 7, -1 },
	 },
	 gp = 300,
	 exp = 10,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44570,
	 destmissionid = 0,
	 precondition = -1,
	 character = 0,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58770, 58780, 58790,  },
	 item = {
		 { 7700, 200, -1 },
		 { 13980, 7, -1 },
	 },
	 gp = 700,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44580,
	 destmissionid = 0,
	 precondition = -1,
	 character = 1,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58771, 58780, 58790,  },
	 item = {
		 { 7710, 200, -1 },
		 { 13990, 7, -1 },
	 },
	 gp = 700,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44590,
	 destmissionid = 0,
	 precondition = -1,
	 character = 2,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58772, 58780, 58790,  },
	 item = {
		 { 7720, 200, -1 },
		 { 14000, 7, -1 },
	 },
	 gp = 700,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44600,
	 destmissionid = 0,
	 precondition = -1,
	 character = 3,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58773, 58780, 58790,  },
	 item = {
		 { 26260, 7, -1 },
		 { 26280, 7, -1 },
	 },
	 gp = 700,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

MissionSystem:AddMission{ 
	 missionid = 44610,
	 destmissionid = 0,
	 precondition = -1,
	 character = 4,
	 promotion = 0,
	 MinLevel = 0,
	 MaxLevel = 75,
	 InvalidMission = {  },
	 submission = { 58774, 58780, 58790,  },
	 item = {
		 { 33340, 7, -1 },
		 { 39610, 7, -1 },
	 },
	 gp = 700,
	 exp = 20,
	 bPromotion = 0,
	 NewCharacterID = -1,
}

