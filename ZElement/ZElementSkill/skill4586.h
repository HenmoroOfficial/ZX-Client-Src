#ifndef __CPPGEN_GNET_SKILL4586
#define __CPPGEN_GNET_SKILL4586
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4586:public Skill
    {
      public:
        enum
        { SKILL_ID = 4586 };
          Skill4586 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4586Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 750;
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
                return 250;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetDarklight (skill->GetPlayer ()->GetDarklight () + 100 * (skill->GetPlayer ()->GetDarklight () >= 0 ? 1 : -1));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4586Stub ():SkillStub (4586)
        {
            occupation = 167;
            name = L"元·无间<玄>";
            nativename = "元·无间<玄>";
            icon = "无间(玄).dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
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
            skill_class = 11;
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
            talent[0] = 4116;
            talent[1] = 4133;
            talent_size = 2;
            action[0] = "轩辕_合一";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "轩辕_合一";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "轩辕_骑乘_合一";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "轩辕_骑乘_合一";
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
        virtual ~ Skill4586Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4037 };
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
            return 120000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
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
                               180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               100, skill->GetLevel (), skill->GetLevel (), 120);

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetDarklight () < 0 ? 120 : 0));
            skill->GetVictim ()->SetTime ((skill->GetPlayer ()->GetCultivation () == 1)
                                          && (skill->GetPlayer ()->GetSkilllevel (4570) >
                                              0) ? (1000 * skill->GetPlayer ()->GetMaxatk () * 0.5 * skill->GetPlayer ()->GetSkilllevel (4570)) : 0);
            skill->GetVictim ()->SetRatio (1000 * (skill->GetPlayer ()->GetSkilllevel (4012) + skill->GetT0 ()));
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (7000 - 1000 * skill->GetPlayer ()->GetSkilllevel (4030));
            skill->GetVictim ()->SetValue (5);
            skill->GetVictim ()->SetDarkuniform (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetDarklight () >= 0 ? 120 : 0));
            skill->GetVictim ()->SetTime ((skill->GetPlayer ()->GetCultivation () == 1)
                                          && (skill->GetPlayer ()->GetSkilllevel (4570) >
                                              0) ? (1000 * skill->GetPlayer ()->GetMaxatk () * 0.5 * skill->GetPlayer ()->GetSkilllevel (4570)) : 0);
            skill->GetVictim ()->SetRatio (1000 * (skill->GetPlayer ()->GetSkilllevel (4013) + skill->GetT1 ()));
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (7000 - 1000 * skill->GetPlayer ()->GetSkilllevel (4031));
            skill->GetVictim ()->SetValue (5);
            skill->GetVictim ()->SetLightuniform (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskillaccu (1);
            skill->GetVictim ()->SetTime (1000 * skill->GetLevel () + 500);
            skill->GetVictim ()->SetImmunesilent (1);
            return true;
        }
#endif
    };
}
#endif
