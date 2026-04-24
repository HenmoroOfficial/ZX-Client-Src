#ifndef __CPPGEN_GNET_SKILL3264
#define __CPPGEN_GNET_SKILL3264
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3264:public Skill
    {
      public:
        enum
        { SKILL_ID = 3264 };
          Skill3264 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3264Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.2 + norm (zrand (130) / 100.0));
                skill->SetCrit (0.3);
                skill->SetCrithurt (0.5);
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                (1.4 * skill->GetLevel () * skill->GetLevel () + 7.9 * skill->GetLevel () + 677) + INT (0.05 * skill->GetLevel () +
                                                                                                                        0.55) * (0.72 *
                                                                                                                                 skill->GetLevel () *
                                                                                                                                 skill->GetLevel () +
                                                                                                                                 62 *
                                                                                                                                 skill->GetLevel () +
                                                                                                                                 348));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3264Stub ():SkillStub (3264)
        {
            occupation = 128;
            name = L"¹íµÀ-±©É³Âş¾í";
            nativename = "¹íµÀ-±©É³Âş¾í";
            icon = "±©É³Âş¾í.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 1;
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
            talent[0] = 1148;
            talent[1] = 1149;
            talent[2] = 1151;
            talent[3] = 1155;
            talent[4] = 1543;
            talent_size = 5;
            action[0] = "¹íµÀ_±©É³Âş¾í";
            action[1] = "¹íµÀ_±©É³Âş¾í";
            action[2] = "¹íµÀ_±©É³Âş¾í";
            action[3] = "¹íµÀ_±©É³Âş¾í";
            action[4] = "¹íµÀ_±©É³Âş¾í";
            action[5] = "¹íµÀ_±©É³Âş¾í";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[16] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[17] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[18] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[19] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[20] = "¹íµÀ_Æï³Ë_±©É³Âş¾í";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 4;
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
#endif
        }
        virtual ~ Skill3264Stub ()
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
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (5);
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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (20000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (3000 + zrand (500));
            skill->GetVictim ()->SetSubdefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.4);
        }
#endif
    };
}
#endif
