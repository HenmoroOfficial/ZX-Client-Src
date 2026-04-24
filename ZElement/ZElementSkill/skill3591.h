#ifndef __CPPGEN_GNET_SKILL3591
#define __CPPGEN_GNET_SKILL3591
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3591:public Skill
    {
      public:
        enum
        { SKILL_ID = 3591 };
          Skill3591 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3591Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3591Stub ():SkillStub (3591)
        {
            occupation = 168;
            name = L"¹íÉñÆø<É·>";
            nativename = "¹íÉñÆø<É·>";
            icon = "Ä§¡¤¹íÉñÆø.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 10;
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
            talent[0] = 3527;
            talent_size = 1;
            action[0] = "Ì«ê»_×çÖä";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "Ì«ê»_×çÖä";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "Ì«ê»_Æï³Ë_×çÖä";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "Ì«ê»_Æï³Ë_×çÖä";
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
            weapon_limit = 11;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3591Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3501 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1050 + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 10000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1050 + 4 * skill->GetLevel () * skill->GetLevel (),
                               180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               90,
                               5 + skill->GetLevel (),
                               5 * skill->GetLevel (),
                               skill->GetLevel (),
                               3 + skill->GetLevel () / 4, 5 * skill->GetLevel (), 3 + skill->GetLevel (), 2 + skill->GetLevel ());

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
                                                 (skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetRes1 () >
                                                  0 ? skill->GetPlayer ()->GetVar1 () - skill->GetPlayer ()->GetRes1 () : 0));
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetGuishen2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (3000 + 1000 * (skill->GetLevel () / 4));
            skill->GetVictim ()->SetValue (skill->GetLevel ());
            skill->GetVictim ()->SetCrippleddebuff (1);
            skill->GetVictim ()->SetTime (2000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (3 + skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * (0.05 * skill->GetLevel () + norm (skill->GetLevel () / 19.99) * 0.2));
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
#endif
    };
}
#endif
