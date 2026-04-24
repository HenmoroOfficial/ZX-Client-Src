--100
lua_glb = {}

function lua_glb:check_md5(mapid)
	local maps = {}
	local files = {}

	--files["Maps\\a11\\a11.ecbsd"] = "A3FF3653054C78B2B79DC18A5B0216D4";
	--files["Maps\\a11\\region.clt"] = "8D4D81F5BF646AA9F52EA682A143DF8C";
	--maps["a11"] = files;

	--files = {}
	--files["Maps\\a16\\a16.ecbsd"] = "47F081DF188456A6CACCBC0A9A904016";
	--files["Maps\\a16\\region.clt"] = "A543B0ACEC5F680B583989001F05420C";
	--maps["a16"] = files;

	--files = {}
	--files["Maps\\a17\\a17.ecbsd"] = "40DD578070D367BD784288E4271DEA8B";
	--files["Maps\\a17\\region.clt"] = "D74636CE3D424F0DAC5665E3CA831B03";
	--maps["a17"] = files;

	--files = {}
	--files["Maps\\a23\\region.clt"] = "B5778FE257DA9BD478202F033BBEF979";
	--maps["a23"] = files;

	--files = {}
	--files["Maps\\a24\\a24.ecbsd"] = "5C404E17276916B06F6B7C0C8892A556";
	--files["Maps\\a24\\region.clt"] = "C89AA26F6AC012482627FFBC013A9A8B";
	--maps["a24"] = files;

	local files_to_check = maps[string.lower(mapid)]
	if files_to_check == nil then
		return true
	end

	local f, m
	for f,m in pairs(files_to_check) do
		md5 = GlobalApi.lua_glb_CalcFileMd5(f)
		if string.lower(m) ~= string.lower(md5) then
			return false
		end	 
	end
	
 	return true
end
