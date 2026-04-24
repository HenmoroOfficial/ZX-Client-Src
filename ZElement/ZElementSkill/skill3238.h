#ifndef __CPPGEN_GNET_SKILL3238
#define __CPPGEN_GNET_SKILL3238
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3238:public Skill
    {
      public:
        enum
        { SKILL_ID = 3238 };
          Skill3238 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3238Stub:public SkillStub
    {
      public:
        Skill3238Stub ():SkillStub (3238)
        {
            occupation = 129;
            name = L"海外任务奖励4状态";
            nativename = "海外任务奖励4状态";
            icon = "";
            maxlevel = 99;
            maxlearn = 10;
            type = 13;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
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
            action[0] = "";
            action[1] = "";
            action[2] = "";
            action[3] = "";
            action[4] = "";
            action[5] = "";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "";
            action[16] = "";
            action[17] = "";
            action[18] = "";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
#endif
        }
        virtual ~ Skill3238Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
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
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 1 ? 120 : 0));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (7);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * 0.01 * 900 >
                                           2000 ? 2000 : skill->GetPlayer ()->GetMaxhp () * 0.01 * 900);
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetBuffid (7);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxmp () * 0.01 * 900 >
                                           2000 ? 2000 : skill->GetPlayer ()->GetMaxmp () * 0.01 * 900);
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 3 ? 120 : 0));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetBuffid (7);
            skill->GetVictim ()->SetValue (99);
            skill->GetVictim ()->SetAdddefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 4 ? 120 : 0));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetRatio (0.02);
            skill->GetVictim ()->SetBuffid (7);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
#endif
    };
}
#endif
