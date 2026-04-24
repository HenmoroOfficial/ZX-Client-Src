#ifndef __CPPGEN_GNET_SKILL2014
#define __CPPGEN_GNET_SKILL2014
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2014:public Skill
    {
      public:
        enum
        { SKILL_ID = 2014 };
          Skill2014 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2014Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2014Stub ():SkillStub (2014)
        {
            occupation = 129;
            name = L"¸´ÖÆ";
            nativename = "¸´ÖÆ";
            icon = "¸´ÖÆ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 0;
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
            preskillnum = 1;
            action[0] = "ÇàÔÆÃÅ_Ììî¸µ¤¶¦";
            action[1] = "ÇàÔÆÃÅ_Ììî¸µ¤¶¦";
            action[2] = "¹íÍõ×Ú_Ììî¸µ¤¶¦";
            action[3] = "ºÏ»¶ÅÉ_Ììî¸µ¤¶¦";
            action[4] = "ÌìÒôËÂ_Ììî¸µ¤¶¦";
            action[5] = "¹íµÀ_Ììî¸µ¤¶¦";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_Ììî¸µ¤¶¦";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ììî¸µ¤¶¦";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ììî¸µ¤¶¦";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ììî¸µ¤¶¦";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ììî¸µ¤¶¦";
            action[20] = "¹íµÀ_Æï³Ë_Ììî¸µ¤¶¦";
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
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 14062;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2014Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1507 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 0, 30);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetRatio (7 * 24 * 3600);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (25382);
            skill->GetVictim ()->SetCreateitem (1);
            return true;
        }
#endif
    };
}
#endif
