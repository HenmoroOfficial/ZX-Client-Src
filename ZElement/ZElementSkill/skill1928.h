#ifndef __CPPGEN_GNET_SKILL1928
#define __CPPGEN_GNET_SKILL1928
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1928:public Skill
    {
      public:
        enum
        { SKILL_ID = 1928 };
          Skill1928 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1928Stub:public SkillStub
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
      Skill1928Stub ():SkillStub (1928)
        {
            occupation = 151;
            name = L"²âÊÔ¼¼ÄÜD";
            nativename = "²âÊÔ¼¼ÄÜD";
            icon = "²âÊÔ4.dds";
            maxlevel = 99;
            maxlearn = 5;
            type = 1;
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
            action[0] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[1] = "ÇàÔÆÃÅ_Ç¬À¤Õ¶";
            action[2] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[3] = "ºÏ»¶ÅÉ_Ç¬À¤Õ¶";
            action[4] = "ÌìÒôËÂ_Ç¬À¤Õ¶";
            action[5] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ç¬À¤Õ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ç¬À¤Õ¶";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ç¬À¤Õ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1928Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 16);
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
            return 1000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 5);
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
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (800);
            skill->GetVictim ()->SetAddattack4 (1);
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (100);
            skill->GetVictim ()->SetAddanti2 (1);
            return true;
        }
#endif
    };
}
#endif
