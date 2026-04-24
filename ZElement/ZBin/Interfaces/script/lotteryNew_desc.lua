Lottery_Desc = {};

	
function Lottery_Desc:GetLotteryDesc(id, dicedata)
	if id == 6576 then	--神木宝盒
		if ZLottery_FindSameNum(dicedata,4)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,4)==2 or ZLottery_FindSameNum(dicedata,3)==3
			or ZLottery_FindSameNum(dicedata,2)==3 or ZLottery_FindSameNum(dicedata,1)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,4)==1 then
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif id == 7916 then	--玄龟金鼎
		if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,3)==3 or 
		(ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
		
	elseif id == 10484 then	--仙工宝箱
		if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==1 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif id == 10722 then	--灵狐元丹
		if ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)>=1 and ZLottery_FindSameNum(dicedata,3)==0 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif id == 11537 then	--玄蛇之心
		if ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)>=1 and ZLottery_FindSameNum(dicedata,3)==0 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif id == 11742 then	--山河扇大礼包
			return "一等奖"
		
	elseif id == 11743 then	--万毒鼎大礼包
			return "一等奖"
			
	elseif id == 12006 then	--龙驹宝鉴
		if ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)>=1 and ZLottery_FindSameNum(dicedata,3)==0 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif id == 13318 then	--绽魂仙羽 
	  if ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)>=1 and ZLottery_FindSameNum(dicedata,3)==0 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "四等奖"
		elseif ZLottery_FindSameNum(dicedata,4)==3 then  
			return "三等奖"		
		else	
			return "五等奖"	
		end		
		
	elseif id == 13377 then	--虚拟币出产箱子的绽魂仙羽
	  if ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)>=1 and ZLottery_FindSameNum(dicedata,3)==0 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "四等奖"
		elseif ZLottery_FindSameNum(dicedata,4)==3 then  
			return "三等奖"		
		else	
			return "五等奖"	
		end		
		
	elseif id == 13319 then	--乾坤遁宝
	  if ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)>=1 and ZLottery_FindSameNum(dicedata,3)==0 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)>=1 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif (id>=13320 and id<=13339) or (id>=18752 and id<=18756) then	--护符材料
			return "一等奖"

	elseif id == 13593 then
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
		
	elseif id == 13905 then	--狼图腾
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
			
	elseif id == 15677 then	--狮王之心
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end					
	
	elseif id == 15866 then	--雷光墨玉
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then  
			return "三等奖"
		else	
			return "四等奖"	
		end				

	elseif id == 16550 then	--逐鹿御令
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==1 and ZLottery_FindSameNum(dicedata,2)==1 and ZLottery_FindSameNum(dicedata,3)==1 then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	
	elseif id == 18283 then	--麒麟金尊
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
	elseif id == 18799 then	--昆仑天晶
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	

	elseif id == 18890 then	--行天圣宝
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
	
	elseif (id==19079 or id==19081 or id==19082 or id==19087) then	--金币宝箱
			return "一等奖"		
	elseif (id==19112 or id==19113 or id==19114 or id==19115) then	--金币宝箱
			return "一等奖"										
	elseif (id>=19425 and id<=19433)  then													--金币宝箱
			return "一等奖"		
	
	elseif id == 19125 then	--神龙秘笈
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	elseif id == 19297 then	--云光天鉴
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	elseif id == 19434 then	--票彩鱼金 (龙宫锦图)
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	elseif id == 19558 then	--如意金鼓
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	elseif id == 19713 then	--妖马炎蹄/龙魂炎晶
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
	elseif id == 20097 then	--08国庆虚拟币彩票
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	elseif id == 20109 then	--鸣珏瑰玉
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end	
	elseif id == 20284 then	--苍穹光印
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0 then  
			return "三等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 or 
		(ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then  
			return "三等奖"
		else	
			return "四等奖"	
		end		
	elseif id == 20480 then	--孔雀令
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2
				or (ZLottery_FindSameNum(dicedata,1)==0 and ZLottery_FindSameNum(dicedata,2)==0) then
			return "三等奖"
		else	
			return "四等奖"	
		end		
	elseif id == 20482 then	--龙皇古卷
	  if ZLottery_FindSameNum(dicedata,1)==3 then
			return "一等奖"
		elseif ZLottery_FindSameNum(dicedata,2)==3 then
			return "二等奖"		
		elseif ZLottery_FindSameNum(dicedata,1)==2 or ZLottery_FindSameNum(dicedata,2)==2 then
			return "三等奖"
		else	
			return "四等奖"	
		end		
	elseif id == 20460 then	--命运罗盘
		if dicedata[2]==1 and dicedata[1]==1 then
			return "一等奖"
		elseif dicedata[2]==1 then
			return "二等奖"
		elseif dicedata[2]==3 then
			return "三等奖"
		else	
			return "四等奖"	
		end
	
	end		
	return ""
end


function ZLottery_FindSameNum(s1,a) --寻找某个数的个数
	local i,j
	j=0
	for i=1,table.getn(s1) do
		if s1[i]==a then 
		j=j+1
		end 
	end 
	return j	
end 
