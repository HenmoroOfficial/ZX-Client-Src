#ifndef __CPPGEN_GNET_SKILL3029
#define __CPPGEN_GNET_SKILL3029
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3029:public Skill
    {
      public:
        enum
        { SKILL_ID = 3029 };
          Skill3029 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3029Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
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
                skill->SetCrit (0.05);
                skill->SetRatio (1.33 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 0.05 * skill->GetT1 ());
                skill->SetPlus (35 * skill->GetT3 () * skill->GetLevel () + 180 + 2 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3029Stub ():SkillStub (3029)
        {
            occupation = 167;
            name = L"æ…√Œ<–˛>";
            nativename = "æ…√Œ<–˛>";
            icon = "œ…°§æ…√Œ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 1;
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
            talent[0] = 724;
            talent[1] = 732;
            talent[2] = 743;
            talent[3] = 765;
            talent_size = 4;
            action[0] = "∫œª∂≈…_æ…√Œ";
            action[1] = "∫œª∂≈…_æ…√Œ";
            action[2] = "∫œª∂≈…_æ…√Œ";
            action[3] = "∫œª∂≈…_æ…√Œ";
            action[4] = "∫œª∂≈…_æ…√Œ";
            action[5] = "∫œª∂≈…_æ…√Œ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "∫œª∂≈…_∆Ô≥À_æ…√Œ";
            action[16] = "∫œª∂≈…_∆Ô≥À_æ…√Œ";
            action[17] = "∫œª∂≈…_∆Ô≥À_æ…√Œ";
            action[18] = "∫œª∂≈…_∆Ô≥À_æ…√Œ";
            action[19] = "∫œª∂≈…_∆Ô≥À_æ…√Œ";
            action[20] = "∫œª∂≈…_∆Ô≥À_æ…√Œ";
            action[21] = "0";
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
#endif
        }
        virtual ~ Skill3029Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 779 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2.3 * skill->GetLevel () * skill->GetLevel () + 3.5 * skill->GetLevel () + 710);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
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
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (2.3 * skill->GetLevel () * skill->GetLevel () + 3.5 * skill->GetLevel () + 710),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               0,
                               5,
                               INT (33 + INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15))),
                               INT (180 + 2 * skill->GetLevel () * skill->GetLevel ()), 14 + skill->GetLevel (), 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * ((60 + 8 * skill->GetT2 ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.48 * (INT (zrand (100) / (100 - 4 * skill->GetT2 ())) + 1));
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * ((14 + skill->GetLevel () + 5 * skill->GetT3 ())));
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (8 * skill->GetT0 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.02 * skill->GetT0 () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetRatio (220 + INT (zrand (99 + 3 * skill->GetLevel ()) / 100) * 174);
            skill->GetVictim ()->SetAmount (220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (293);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (2000);
            skill->GetVictim ()->SetReduceskillcd (1);
            return true;
        }
#endif
    };
}
#endif
