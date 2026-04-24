#ifndef __CPPGEN_GNET_SKILL2049
#define __CPPGEN_GNET_SKILL2049
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2049:public Skill
    {
      public:
        enum
        { SKILL_ID = 2049 };
          Skill2049 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2049Stub:public SkillStub
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes1 () + 2 * skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetVar2 () * 10);
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
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
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes1 () + 2 * skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetVar2 () * 10);
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
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
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes1 () + 2 * skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetVar2 () * 10);
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (4);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes1 () + 2 * skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->SetPlus (skill->GetPlayer ()->GetVar2 () * 10);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2049Stub ():SkillStub (2049)
        {
            occupation = 148;
            name = L"ÐýÁú»ÃÉ±II";
            nativename = "ÐýÁú»ÃÉ±II";
            icon = "ÐýÁú»ÃÉ±II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 9;
            clearmask = 129;
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
            preskillnum = 1;
            talent[0] = 525;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[1] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[2] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[3] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[4] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[5] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[6] = "¹íÍõ×Ú_ÐýÁú»ÃÉ±";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
            action[21] = "¹íÍõ×Ú_Æï³Ë_ÐýÁú»ÃÉ±";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill2049Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 525 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (150 * skill->GetLevel () + 66);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 4000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 40000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               150 * skill->GetLevel () + 66,
                               2 * skill->GetLevel (),
                               2 * skill->GetLevel (),
                               4 * skill->GetLevel (),
                               6 * skill->GetLevel (),
                               8 * skill->GetLevel (), 3 + 3 * skill->GetLevel (), 3, 9 + 3 * skill->GetLevel (), 8, skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (3 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3100);
            skill->GetVictim ()->SetBarehanded (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 ()));
            skill->GetVictim ()->SetTime (1000 * skill->GetPlayer ()->GetVar1 () + 1000 * int (skill->GetT0 () / 4.99));
              skill->GetVictim ()->SetDizzy (1);
              return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (9 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (8000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.01 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpgen (1);
            return true;
        }
#endif
    };
}
#endif
