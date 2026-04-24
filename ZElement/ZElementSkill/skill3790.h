#ifndef __CPPGEN_GNET_SKILL3790
#define __CPPGEN_GNET_SKILL3790
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3790:public Skill
    {
      public:
        enum
        { SKILL_ID = 3790 };
          Skill3790 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3790Stub:public SkillStub
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
      Skill3790Stub ():SkillStub (3790)
        {
            occupation = 146;
            name = L"ÄÎºÎ£º·Ù";
            nativename = "ÄÎºÎ£º·Ù";
            icon = "ÄÎºÎ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 2;
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
            skill_class = 0;
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
            action[0] = "¹íÍõ×Ú_ÄÎºÎ";
            action[1] = "¹íÍõ×Ú_ÄÎºÎ";
            action[2] = "¹íÍõ×Ú_ÄÎºÎ";
            action[3] = "¹íÍõ×Ú_ÄÎºÎ";
            action[4] = "¹íÍõ×Ú_ÄÎºÎ";
            action[5] = "¹íÍõ×Ú_ÄÎºÎ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÄÎºÎ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÄÎºÎ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÄÎºÎ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÄÎºÎ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÄÎºÎ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÄÎºÎ";
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
            rangetype = 2;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3790Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (800);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 800);

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
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetRatio (0.1);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetIncattack (1);
            skill->GetVictim ()->SetTime (15000 + 100);
            skill->GetVictim ()->SetValue (20 * (1 + norm (zrand (100) / 75)));
            skill->GetVictim ()->SetBlessed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (60000 + 100);
            skill->GetVictim ()->SetRatio (0.15001);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetIncskillaccu (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
