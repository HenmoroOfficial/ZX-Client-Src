#ifndef __CPPGEN_GNET_SKILL3311
#define __CPPGEN_GNET_SKILL3311
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3311:public Skill
    {
      public:
        enum
        { SKILL_ID = 3311 };
          Skill3311 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3311Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0.48 + 0.02 * 3 + 0.02 * 3);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3311Stub ():SkillStub (3311)
        {
            occupation = 128;
            name = L"天选道怨";
            nativename = "天选道怨";
            icon = "天选道怨.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            action[0] = "天选道怨";
            action[1] = "天选道怨";
            action[2] = "天选道怨";
            action[3] = "天选道怨";
            action[4] = "天选道怨";
            action[5] = "天选道怨";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "天选道怨";
            action[16] = "天选道怨";
            action[17] = "天选道怨";
            action[18] = "天选道怨";
            action[19] = "天选道怨";
            action[20] = "天选道怨";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "火焰之环";
            rangetype = 3;
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
        virtual ~ Skill3311Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1000, 15, 48 + 2 * 3, 20, 60, 2 * 3, 5 * 3, 2 * 3, 2000 * 3, 5 * 3, 5 * 3, 3 * 3);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel () + 0.03 * 3) * (1 + 0.1 * 3) -
                                                  skill->GetPlayer ()->GetSaint () >
                                                  15 ? 100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel () + 0.03 * 3) * (1 + 0.1 * 3) -
                                                  skill->GetPlayer ()->GetSaint () : 15));
            skill->GetVictim ()->SetTime (60000 - 2000 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((2 * 3 * skill->GetPlayer ()->GetMaxhp ()) * (1 + 0.1 * 3) +
                                            0.005 * 3 * 3 * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (2000 * 3);
            skill->GetVictim ()->SetHpleak3 (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * 3 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3000 * 3 + 100 + 2000 * 3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (10000);
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * 3 + 2 * 3));
            skill->GetVictim ()->SetTime (2000 * 3 + 100 + 2000 * 3);
            skill->GetVictim ()->SetRatio (0.2);
            skill->GetVictim ()->SetValue (0.2);
            skill->GetVictim ()->SetRandcurse (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8100);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (0.004 * skill->GetPlayer ()->GetMp () + 1);
            skill->GetVictim ()->SetAdddefence (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
