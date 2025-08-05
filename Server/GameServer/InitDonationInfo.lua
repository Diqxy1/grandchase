
BeginTime       	= { 2009, 10, 08, 00 }   -- Y, M, D, H
EndTime         	= { 2009, 10, 20, 00 }   -- Y, M, D, H
DisplayTime     	= { 2009, 10, 20, 00 }   -- Y, M, D, H
TickGap         	= 1000 * 60         -- ms
SyncDonationGap 	= 1000 * 60 * 5     -- ms
DonationPointType       = 198180              -- 기부할 포인트?? ( 0 은 GP, 0 보다 크면 Item )
MinPoint        	= 1                 -- 최소 기부액
UserReward      	= { Point = 1, Reward = {} }  -- Donation Point, {ItemID, Factor,...}
DonationReward  	= { Point = 99999999, Reward = {} }     -- Donation Point, {ItemID, Factor,...}
MaxGrade		= 5
