#ifndef __CPPGEN_GNET_SKILL278
#define __CPPGEN_GNET_SKILL278
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill278:public Skill
    {
      public:
        enum
        { SKILL_ID = 278 };
          Skill278 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill278Stub:public SkillStub
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
                skill->SetRatio (1.04 + 0.07 * skill->GetT3 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) * (6 * skill->GetLevel () + 203) +
                              INT (0.05 * skill->GetLevel () + 0.55) * (0.21 * skill->GetLevel () * skill->GetLevel () + 26 * skill->GetLevel () +
                                                                        50)) * (1 + 0.3 * skill->GetT1 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill278Stub ():SkillStub (278)
        {
            occupation = 11;
            name = L"金刚经";
            nativename = "金刚经";
            icon = "金刚经.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 3;
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
            talent[0] = 676;
            talent[1] = 693;
            talent[2] = 698;
            talent[3] = 702;
            talent_size = 4;
            action[0] = "天音寺_金刚经";
            action[1] = "天音寺_金刚经";
            action[2] = "天音寺_金刚经";
            action[3] = "天音寺_金刚经";
            action[4] = "天音寺_金刚经";
            action[5] = "天音寺_金刚经";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_金刚经";
            action[16] = "天音寺_骑乘_金刚经";
            action[17] = "天音寺_骑乘_金刚经";
            action[18] = "天音寺_骑乘_金刚经";
            action[19] = "天音寺_骑乘_金刚经";
            action[20] = "天音寺_骑乘_金刚经";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
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
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill278Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 279 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (40);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 60, 62, 64, 66, 68, 70, 72, 74, 76 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               40,
                               4,
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (6 * skill->GetLevel () + 203) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.21 * skill->GetLevel () * skill->GetLevel () +
                                                                              26 * skill->GetLevel () + 50)));

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
            skill->GetVictim ()->SetProbability (1.0 * (6 * skill->GetT2 ()));
            skill->GetVictim ()->SetTime (4050 * skill->GetT2 ());
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (8000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (2 * skill->GetT0 () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
