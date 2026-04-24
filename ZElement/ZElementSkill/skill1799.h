#ifndef __CPPGEN_GNET_SKILL1799
#define __CPPGEN_GNET_SKILL1799
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1799:public Skill
    {
      public:
        enum
        { SKILL_ID = 1799 };
          Skill1799 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1799Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.05);
                skill->SetPlus (1500);
                skill->SetRatio (1.3);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1799Stub ():SkillStub (1799)
        {
            occupation = 129;
            name = L"ÌØ¼¼£ºÌìÄ§ÈĞ";
            nativename = "ÌØ¼¼£ºÌìÄ§ÈĞ";
            icon = "ÌìÄ§ÈĞ.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[1] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[2] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[3] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[4] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[5] = "¹íÍõ×Ú_ÊÉÑªÕ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÊÉÑªÕ¶";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "ÆÕÍ¨¹¥»÷1";
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
            skill_limit = 10;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1799Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1200);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (6);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1200);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1);

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.3 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
