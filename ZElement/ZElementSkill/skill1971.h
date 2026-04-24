#ifndef __CPPGEN_GNET_SKILL1971
#define __CPPGEN_GNET_SKILL1971
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1971:public Skill
    {
      public:
        enum
        { SKILL_ID = 1971 };
          Skill1971 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1971Stub:public SkillStub
    {
      public:
        Skill1971Stub ():SkillStub (1971)
        {
            occupation = 166;
            name = L"天人物语";
            nativename = "天人物语";
            icon = "人族物语.dds";
            maxlevel = 20;
            maxlearn = 10;
            type = 11;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 256;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
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
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            use_proficiency = true;
            inc_proficiency = 0;
            skill_limit = 0;
#ifdef _SKILL_SERVER
#endif
        }
        virtual ~ Skill1971Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 };
            return array[level - 1];
        }
        int GetRequiredProficiency (int level) const
        {
            static int array[10] = { 0, 6, 48, 144, 432, 864, 2592, 5184, 9216, 14256 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               skill->GetShowlevel (),
                               skill->GetCurproficiency (),
                               skill->GetNeedproficiency (), skill->GetLevel () < 1 ? table.Find (19713) : table.Find (19714));

        }
#endif
#ifdef _SKILL_SERVER
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasincpetdamage (0.0501 * norm (skill->GetLevel () / 9.9));
            skill->GetPlayer ()->SetPasdecpethurt (0.1005 * norm (skill->GetLevel () / 9.9));
            return true;
        }
#endif
    };
}
#endif
