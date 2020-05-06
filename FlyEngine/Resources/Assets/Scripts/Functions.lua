function Just123(number)
    	if(number<33) then 
    		return 1
    	end
    	if(number<66) then
    		return 2 
    	end
    	return 3
    end


function GradeToGPA(grade)
	if(grade<65) then return "F" 	end
	if(grade<67) then return "D"	end
	if(grade<70) then return "C-"	end
	if(grade<73) then return "C"	end
	if(grade<80) then return "C+"	end
	if(grade<83) then return "B-"	end
	if(grade<87) then return "B"	end
	if(grade<90) then return "B+"	end
	if(grade<93) then return "A-"	end
	if(grade<97) then return "A"	end
	 return "A+"
end
