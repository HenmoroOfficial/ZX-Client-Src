#ifndef __CPPGEN_GNET_SKILL3445
#define __CPPGEN_GNET_SKILL3445
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3445:public Skill
    {
      public:
        enum
        { SKILL_ID = 3445 };
          Skill3445 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3445Stub:public SkillStub
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
                skill->GetPlayer ()->
                    SetVar2 (int
                             ((skill->GetPlayer ()->GetRes1 () + skill->GetPlayer ()->GetRes2 () + skill->GetPlayer ()->GetRes3 () +
                               skill->GetPlayer ()->GetRes4 () + skill->GetPlayer ()->GetRes5 ()) / 5));
                  skill->GetPlayer ()->SetVar3 (int (skill->GetPlayer ()->GetVar2 () / 10));
                  skill->GetPlayer ()->SetVar1 (int (skill->GetPlayer ()->GetHasbuff (4199) == 1 ? 1 : 0) * 1 +
                                                int (skill->GetPlayer ()->GetHasbuff (4200) ==
                                                     1 ? 1 : 0) * 2 + int (skill->GetPlayer ()->GetHasbuff (4201) ==
                                                                           1 ? 1 : 0) * 3 + int (skill->GetPlayer ()->GetHasbuff (4202) ==
                                                                                                 1 ? 1 : 0) * 4);
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3445Stub ():SkillStub (3445)
        {
            occupation = 67;
            name = L"·¨Ôª½â·Å";
            nativename = "·¨Ôª½â·Å";
            icon = "ÁéÁ¦Àí½â.dds";
            maxlevel = 20;
            maxlearn = 8;
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
            preskillnum = 1;
            talent[0] = 3404;
            talent[1] = 3368;
            talent[2] = 3369;
            talent[3] = 3428;
            talent[4] = 3429;
            talent[5] = 3747;
            talent_size = 6;
            action[0] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[1] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[2] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[3] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[4] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[5] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[6] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
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
        virtual ~ Skill3445Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3442 };
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
            return 90000 - 5000 * skill->GetT0 () - 90000 * 0.03 * skill->GetT5 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 106, 109, 113, 117, 121, 125, 125, 125 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format,
                               560 + 10 * skill->GetLevel (), (10 + 3 * skill->GetLevel ()), 2 + skill->GetLevel (), 10 * skill->GetLevel (), 90);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetHasbuff (4199) == 1 || skill->GetPlayer ()->GetHasbuff (4200) == 1
                                                  || skill->GetPlayer ()->GetHasbuff (4201) == 1
                                                  || skill->GetPlayer ()->GetHasbuff (4202) == 1 ? 120 : 0));
            skill->GetVictim ()->SetRatio (4199);
            skill->GetVictim ()->SetAmount (4200);
            skill->GetVictim ()->SetValue (4201);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (10000 + 1000 * 3 * skill->GetLevel () + 1000 * 3 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (2 + skill->GetLevel () + skill->GetT1 () + skill->GetT2 () + skill->GetT3 () + skill->GetT4 ());
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () + 10 * skill->GetPlayer ()->GetVar3 () + 10 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (3611);
            skill->GetVictim ()->SetAddcommon (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetHasbuff (4199) == 1 || skill->GetPlayer ()->GetHasbuff (4200) == 1
                                                  || skill->GetPlayer ()->GetHasbuff (4201) == 1
                                                  || skill->GetPlayer ()->GetHasbuff (4202) == 1 ? 120 : 0));
            skill->GetVictim ()->SetRatio (4202);
            skill->GetVictim ()->SetAmount (4202);
            skill->GetVictim ()->SetValue (4202);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (10000 + 1000 * 3 * skill->GetLevel () + 1000 * 3 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (2 + skill->GetLevel () + skill->GetT1 () + skill->GetT2 () + skill->GetT3 () + skill->GetT4 ());
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () + 10 * skill->GetPlayer ()->GetVar3 () + 10 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (3611);
            skill->GetVictim ()->SetAddcommon (1);
            return true;
        }
#endif
    };
}
#endif
