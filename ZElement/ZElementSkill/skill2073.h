#ifndef __CPPGEN_GNET_SKILL2073
#define __CPPGEN_GNET_SKILL2073
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2073:public Skill
    {
      public:
        enum
        { SKILL_ID = 2073 };
          Skill2073 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2073Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 900;
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes1 ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.05 * skill->GetT0 ());
                skill->SetPlus (0.3 * skill->GetLevel () * skill->GetPlayer ()->GetRes1 ());
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes3 ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.05 * skill->GetT0 ());
                skill->SetPlus (0.3 * skill->GetLevel () * skill->GetPlayer ()->GetRes3 ());
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetRes5 ());
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.05 * skill->GetT0 ());
                skill->SetPlus (0.3 * skill->GetLevel () * skill->GetPlayer ()->GetRes5 ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill2073Stub ():SkillStub (2073)
        {
            occupation = 150;
            name = L"É­ÂŞÈıĞ¦II";
            nativename = "É­ÂŞÈıĞ¦II";
            icon = "É­ÂŞÈıĞ¦II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 2;
            credittype = 11;
            clearmask = 132;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 4;
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
            talent[0] = 1172;
            talent_size = 1;
            action[0] = "É­ÂŞÈıĞ¦";
            action[1] = "É­ÂŞÈıĞ¦";
            action[2] = "É­ÂŞÈıĞ¦";
            action[3] = "É­ÂŞÈıĞ¦";
            action[4] = "É­ÂŞÈıĞ¦";
            action[5] = "É­ÂŞÈıĞ¦";
            action[6] = "É­ÂŞÈıĞ¦";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "É­ÂŞÈıĞ¦";
            action[16] = "É­ÂŞÈıĞ¦";
            action[17] = "É­ÂŞÈıĞ¦";
            action[18] = "É­ÂŞÈıĞ¦";
            action[19] = "É­ÂŞÈıĞ¦";
            action[20] = "É­ÂŞÈıĞ¦";
            action[21] = "É­ÂŞÈıĞ¦";
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
        virtual ~ Skill2073Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1129 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 10 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (735 + 65 * skill->GetLevel () + 3.3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
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
                               INT (735 + 65 * skill->GetLevel () + 3.3 * skill->GetLevel () * skill->GetLevel ()),
                               2 * skill->GetLevel (), skill->GetLevel (), skill->GetLevel (), skill->GetLevel (), 0.3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () ==
                                           1 ? (skill->GetPlayer ()->GetVar2 () >
                                                skill->GetPlayer ()->GetRes1 () + 1 ? (skill->GetPlayer ()->GetVar2 () -
                                                                                       skill->GetPlayer ()->GetRes1 ()) *
                                                skill->GetLevel () : 0) : (skill->GetPlayer ()->GetVar1 () == 2 ? (skill->GetPlayer ()->GetVar2 () >
                                                                                                                   skill->GetPlayer ()->GetRes3 () +
                                                                                                                   1 ? (skill->GetPlayer ()->
                                                                                                                        GetVar2 () -
                                                                                                                        skill->GetPlayer ()->
                                                                                                                        GetRes3 ()) *
                                                                                                                   skill->GetLevel () : 0) : (skill->
                                                                                                                                              GetPlayer
                                                                                                                                              ()->
                                                                                                                                              GetVar1
                                                                                                                                              () ==
                                                                                                                                              3
                                                                                                                                              ?
                                                                                                                                              (skill->
                                                                                                                                               GetPlayer
                                                                                                                                               ()->
                                                                                                                                               GetVar2
                                                                                                                                               () >
                                                                                                                                               skill->
                                                                                                                                               GetPlayer
                                                                                                                                               ()->
                                                                                                                                               GetRes5
                                                                                                                                               () +
                                                                                                                                               1
                                                                                                                                               ?
                                                                                                                                               (skill->
                                                                                                                                                GetPlayer
                                                                                                                                                ()->
                                                                                                                                                GetVar2
                                                                                                                                                () -
                                                                                                                                                skill->
                                                                                                                                                GetPlayer
                                                                                                                                                ()->
                                                                                                                                                GetRes5
                                                                                                                                                ()) *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () : 0)
                                                                                                                                              : 0)));
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
