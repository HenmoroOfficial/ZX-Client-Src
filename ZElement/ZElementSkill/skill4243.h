#ifndef __CPPGEN_GNET_SKILL4243
#define __CPPGEN_GNET_SKILL4243
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4243:public Skill
    {
      public:
        enum
        { SKILL_ID = 4243 };
          Skill4243 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4243Stub:public SkillStub
    {
      public:
        Skill4243Stub ():SkillStub (4243)
        {
            occupation = 130;
            name = L"浮光阑影2";
            nativename = "浮光阑影2";
            icon = "";
            maxlevel = 5;
            maxlearn = 5;
            type = 3;
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
            rangetype = 0;
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
        virtual ~ Skill4243Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               20 + 6 * skill->GetLevel (),
                               2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () +
                               6 * skill->GetLevel (), 1 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (zrand (100) > 20 + 6 * skill->GetLevel ()? 0 : 600));
            skill->GetVictim ()->SetValue (3);
            skill->GetVictim ()->SetRepel (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetType () ==
                                                  0 ? (zrand (100) >
                                                       20 +
                                                       6 * skill->GetLevel ()? 0 : (2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel () +
                                                                                    skill->GetLevel () * skill->GetLevel () +
                                                                                    6 * skill->GetLevel ())) : 0));
            skill->GetVictim ()->SetTime (1000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
#endif
    };
}
#endif
