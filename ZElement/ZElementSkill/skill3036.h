#ifndef __CPPGEN_GNET_SKILL3036
#define __CPPGEN_GNET_SKILL3036
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3036:public Skill
    {
      public:
        enum
        { SKILL_ID = 3036 };
          Skill3036 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3036Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxhp ());
                skill->SetRatio (0.48 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3036Stub ():SkillStub (3036)
        {
            occupation = 167;
            name = L"天选道怨<玄>";
            nativename = "天选道怨<玄>";
            icon = "仙·天选道怨.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            talent[0] = 1185;
            talent[1] = 1187;
            talent[2] = 1189;
            talent_size = 3;
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
        virtual ~ Skill3036Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1134 };
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
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (420 + 420 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 12000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format,
                               420 + 420 * skill->GetLevel (),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               12,
                               INT (6 + INT (skill->GetLevel () / 2)),
                               48 + 2 * skill->GetLevel (),
                               20,
                               60,
                               1 + 0.1 * skill->GetLevel (),
                               2 + 0.8 * skill->GetLevel (),
                               5 + skill->GetLevel (),
                               1 + 0.5 * skill->GetLevel (),
                               1.8 + 0.1 * skill->GetLevel (),
                               5 + skill->GetLevel (),
                               5 * skill->GetLevel (), 3 * skill->GetLevel (), 5 * skill->GetLevel (), skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.003 * skill->GetPlayer ()->GetLevel ()) * (1 + 0.1 * skill->GetT2 ())));
            skill->GetVictim ()->SetTime (60000 - 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->
                SetAmount (((1 + 0.1 * skill->GetLevel ()) * skill->GetPlayer ()->GetVar2 () +
                            (2 + 0.8 * skill->GetLevel ()) * skill->GetPlayer ()->GetVar1 ()) * (1 + 0.1 * skill->GetT0 ()) +
                           0.005 * skill->GetT0 () * skill->GetLevel () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * (1.8 + 0.1 * skill->GetLevel ()));
            skill->GetVictim ()->SetHpleak3 (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((5 + skill->GetLevel ()) * skill->GetLevel () * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel () + 100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (10000);
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetTime (1000 + 500 * skill->GetLevel () + 100 + 2000 * skill->GetT1 ());
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
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetRes4 () * 0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetFrenzied (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6 + INT (skill->GetLevel () / 2));
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
