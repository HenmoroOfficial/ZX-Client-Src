--用于client中排行榜的显示, 参考相应的word文档
	
rank_disp = {};

---参数：排行榜类型，本次排名，上次排名，名字（玩家或者帮派），职业，排名值
---例如：
---result =  rank_disp:GetDescription(1, 2, 1, "abc", 6, 20)
---返回值的格式请参考肖洲的文档

function rank_disp:GetDescription(id, rank, last_rank, name, prof, val,val2)
	---下面的内容需要修改
	---local rt = "1\t^00FF00↑1\tAAA\t散人\t20级"
	local tprof = {
	   "少侠","鬼王宗一重","鬼王宗二重","鬼王宗三重","合欢派一重","合欢派二重","合欢派三重","青云门一重","青云门二重", "青云门三重",
	   "天音寺一重", "天音寺二重","天音寺三重","鬼王宗四重","鬼王宗五重","鬼王宗六重","合欢派四重","合欢派五重","合欢派六重",
	   "青云门四重","青云门五重","青云门六重","天音寺四重","天音寺五重","天音寺六重","鬼道一重","鬼道二重","鬼道三重","鬼道四重","鬼道五重",
	   "鬼道六重"," ","神裔少侠","九黎一重","九黎二重","九黎三重","九黎四重","九黎五重","九黎六重","烈山一重","烈山二重","烈山三重",
	   "烈山四重","烈山五重","烈山六重","怀光一重","怀光二重","怀光三重","怀光四重","怀光五重","怀光六重","天华一重","天华二重","天华三重",
	   "天华四重","天华五重","辰皇一重","辰皇二重","辰皇三重","辰皇四重","辰皇五重"," "," "," ","焚香一重","焚香二重","焚香三重","焚香四重","焚香五重","焚香六重"," ", 
	    " "," ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "太昊一重",
	    "太昊二重","太昊三重","太昊四重","太昊五重","太昊六重"
	   }
	local rt;
	local zx_reborntimes,zx_level,rankhour,rankminute,ranksecond;
	if id <= 30 then
	if id == 1 then
       		zx_reborntimes=math.ceil(val/200)-1;       
		zx_level=val-zx_reborntimes*200;
       
	   	if zx_reborntimes ==0 then 
	   		if last_rank < 0 then
				rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t"..zx_level.."级"
			else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t"..zx_level.."级"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t"..zx_level.."级"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t"..zx_level.."级"
				end
			end
		elseif zx_reborntimes ==1 then
			if last_rank < 0 then
			rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t".."飞升"..zx_level.."级"
			else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t".."飞升"..zx_level.."级"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t".."飞升"..zx_level.."级"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t".."飞升"..zx_level.."级"
				end
			end
		elseif zx_reborntimes ==2 then
			if last_rank < 0 then
			rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t".."轮回"..zx_level.."级"
			else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t".."轮回"..zx_level.."级"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t".."轮回"..zx_level.."级"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t".."轮回"..zx_level.."级"
				end
			end
		else 
			if last_rank < 0 then
			rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t".."封神"..zx_level.."级"
			else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t".."封神"..zx_level.."级"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t".."封神"..zx_level.."级"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t".."封神"..zx_level.."级"
				end
			end
		end
	end

	    
	    
	if id == 2 then
		if last_rank < 0 then
			rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t"..val.."金"
				else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val.."金"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t"..val.."金"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val.."金"
				end
			end
		end
			
		if id >= 3 then
			if last_rank < 0 then
				rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t"..val
			else 
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t"..val
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val 	
				end
	   		end    
		end
	end   
 
	    
	if id > 30 then
		if id == 31 then
			if last_rank < 0 then
        		rt = rank.."\t^80FF00".."新".."\t"..name.."\t".."".."\t"..(val+1).."级"
        		else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t".."".."\t"..(val+1).."级"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t".."".."\t"..(val+1).."级"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t".."".."\t"..(val+1).."级"
				end
			end
		end	
			
		if id == 32 then
			if last_rank < 0 then
        		rt = rank.."\t^80FF00".."新".."\t"..name.."\t".."".."\t"..val.."金"
        		else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t".."".."\t"..val.."金"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t".."".."\t"..val.."金"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t".."".."\t"..val.."金"
				end	
			end
		end
				
	   	if id == 33 then
	   		if last_rank < 0 then
        		rt = rank.."\t^80FF00".."新".."\t"..name.."\t".."".."\t"..val.."人"
        		else
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t".."".."\t"..val.."人"
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t".."".."\t"..val.."人"
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t".."".."\t"..val.."人"
				end
			end
		end
		
		if id >= 34 and id<80 then
			if last_rank < 0 then
        		rt = rank.."\t^80FF00".."新".."\t"..name.."\t".."".."\t"..val
			else 
				if rank < last_rank then
					rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t".."".."\t"..val
				end
				if rank == last_rank then
					rt = rank.."\t^FFFFFF--".."\t"..name.."\t".."".."\t"..val
				end
				if rank > last_rank then
					rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t".."".."\t"..val 	
	     			end
			end    
		end	
		if id >= 80 and id<=83  then
        		rt = name.."\t"..val
		end	
	end
	if id >= 90 and id <= 93 then
		if last_rank < 0 and val > 0 then
			 rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t"..val.."级"
		else
			if rank < last_rank and val > 0 then
				rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val.."级"
			end
			if rank == last_rank and val > 0 then
				rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t"..val.."级"
			end
			if rank > last_rank and val > 0 then
				rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val.."级"
			end
		end
	end
--id=95,爬塔参数（id,rank,name,prof,val-层数,val2-时间）
	if id == 95 then
	    
	    rankhour=math.floor(val2/3600);
	    rankminute=math.floor((val2-rankhour*3600)/60);
	    ranksecond=val2-rankhour*3600-rankminute*60;
	    
		if last_rank < 0 and val > 0 and val2 >= 0 then
			 rt = rank.."\t^80FF00".."新".."\t"..name.."\t"..tprof[prof+1].."\t"..val.."层".." "..rankhour..":"..rankminute..":"..ranksecond
		else
			if rank < last_rank and val > 0 and val2 >= 0 then
				rt = rank.."\t^80FF00↑"..(last_rank - rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val.."层".." "..rankhour..":"..rankminute..":"..ranksecond
			end
			if rank == last_rank and val > 0 and val2 >= 0 then
				rt = rank.."\t^FFFFFF--".."\t"..name.."\t"..tprof[prof+1].."\t"..val.."层".." "..rankhour..":"..rankminute..":"..ranksecond
			end
			if rank > last_rank and val > 0 and val2 >= 0 then
				rt = rank.."\t^FFFF00↓"..(rank - last_rank).."\t"..name.."\t"..tprof[prof+1].."\t"..val.."层".." "..rankhour..":"..rankminute..":"..ranksecond
			end
		end
	end	  
	return rt;
	
end
