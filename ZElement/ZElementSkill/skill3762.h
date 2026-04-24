#ifndef __CPPGEN_GNET_SKILL3762
#define __CPPGEN_GNET_SKILL3762
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3762:public Skill
    {
      public:
        enum
        { SKILL_ID = 3762 };
          Skill3762 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3762Stub:public SkillStub
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
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->SetRatio (1 + 0.5 + zrand (100) * 0.01);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->SetRatio (1 + 0.5 + zrand (100) * 0.01);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->SetRatio (1 + 0.5 + zrand (100) * 0.01);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3762Stub ():SkillStub (3762)
        {
            occupation = 146;
            name = L"É½ºÓÄæ£º·Ù";
            nativename = "É½ºÓÄæ£º·Ù";
            icon = "É½ºÓÄæ·Ù.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
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
            action[0] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[1] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[2] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[3] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[4] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[5] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[6] = "·ÙÏã¹È_É½ºÓÄæ£ºĞş";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[16] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[17] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[18] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[19] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[20] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[21] = "·ÙÏã¹È_Æï³Ë_É½ºÓÄæ£ºĞş";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
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
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3762Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 360000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (16);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2000);

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
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetBreakcasting (1);
            skill->GetVictim ()->SetTime (zrand (100) < 15 ? 16000 : 0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetRes2 ());
            skill->GetVictim ()->SetSubantiweak (1);
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetRes4 ());
            skill->GetVictim ()->SetSubantisilent (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (36);
        }
#endif
    };
}
#endif
