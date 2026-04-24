#ifndef __CPPGEN_GNET_SKILL877
#define __CPPGEN_GNET_SKILL877
namespace GNET 
{
    
#ifdef _SKILL_SERVER
    class Skill877:public Skill 
    {
      
public:
enum
        { SKILL_ID = 877 };
         
Skill877 ():Skill (SKILL_ID)
        {
    } 
};
    
#endif /* 
 */
  class Skill877Stub:public SkillStub 
    {
      
public: 
Skill877Stub ():SkillStub (877)
        {
            occupation = 128;
            
name = L"Boss天忍王宠D";
            
nativename = "Boss天忍王宠D";
            
icon = "";
            
maxlevel = 100;
            
maxlearn = 10;
            
type = 2;
            
eventflag = 0;
            
baseid = 0;
            
basesp = 0;
            
spcost = 1;
            
timetype = 0;
            
targettype = 0;
            
autoattack = 0;
            
action[0] = "0";
            
action[1] = "0";
            
action[2] = "0";
            
action[3] = "0";
            
action[4] = "0";
            
action[5] = "0";
            
action[6] = "0";
            
action[7] = "0";
            
action[8] = "0";
            
action[9] = "0";
            
action[10] = "0";
            
action[11] = "0";
            
action[12] = "技能攻击4";
            
rangetype = 5;
            
doenchant = false;
            
dobless = false;
            
#ifdef _SKILL_SERVER
#endif /* 
 */
        } 
virtual ~ Skill877Stub ()
        {
        } 
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format) const
        {
            
return _snwprintf (buffer, length, format);
        
}
         
#endif                          /* 
 */
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format) const
        {
            
return _snwprintf (buffer, length, format);
        
}
         
#endif                          /* 
 */
#ifdef _SKILL_SERVER
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0);
        } 
#endif                          /* 
 */
    };
} 
#endif /* 
 */
