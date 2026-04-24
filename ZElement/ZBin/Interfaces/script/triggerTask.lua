-- 触发任务 特定的事件id可以触发特定的任务

trigger_task = {}

--trigger_task['31043'] = { eventid = 31043, eventarg = 3, taskid = 12345 }



function trigger_task:GetTirggerTask()
	return self;
end
	
complete_task = {}

-- 特定的事件可以完成特定的任务，事件可以附带参数，

--complete_task['31043'] = { eventid = 31043, eventarg = 3, taskid = 12345 }
--complete_task['31043'] = { eventid = 31043, eventarg = 3, taskid = 65412 }




function complete_task:GetCompleteTask()
	return self;
end
	

triggerTaskIndex = {}

-- 由触发开启的任务 taskid 对应的 索引，从1开始向后累计
--triggerTaskIndex['12345'] = { taskid = 31043, index = 1}

function triggerTaskIndex:GetTriggerTaskIndex()
    return self;
end

completeTaskIndex = {}
-- 由触发完成的任务 taskid 对应的 索引，从1开始向后累计
--completeTaskIndex['12345'] = {taskid = 12345, index = 1}


function completeTaskIndex:GetCompleteTaskIndex()
	return self;
end