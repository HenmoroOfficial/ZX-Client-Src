#ifndef __CPPGEN_GNET_SKILL3498
#define __CPPGEN_GNET_SKILL3498
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3498:public Skill
    {
      public:
        enum
        { SKILL_ID = 3498 };
          Skill3498 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3498Stub:public SkillStub
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
                skill->SetPlus (700 + 27 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * 12);
                skill->SetRatio (1.05 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3498Stub ():SkillStub (3498)
        {
            occupation = 100;
            name = L"ÈýÉúÅÐ";
            nativename = "ÈýÉúÅÐ";
            icon = "ÈýÉúÅÐ.dds";
            maxlevel = 8;
            maxlearn = 5;
            type = 1;
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
            talent[0] = 3523;
            talent_size = 1;
            action[0] = "Ì«ê»_Ñ¸À×´¸";
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
            action[11] = "Ì«ê»_Ñ¸À×´¸";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "Ì«ê»_Æï³Ë_Ñ¸À×´¸";
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
            action[26] = "Ì«ê»_Æï³Ë_Ñ¸À×´¸";
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
        virtual ~ Skill3498Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (285 + 5 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 135, 136, 137, 138, 139 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               285 + 5 * skill->GetLevel () + 7 * skill->GetLevel () * skill->GetLevel (),
                               700 + 27 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel () * 12,
                               5 + 2 * skill->GetLevel (), 6 * skill->GetLevel (), 2 * skill->GetLevel (), 6 * skill->GetLevel (), 2);

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
            skill->GetVictim ()->SetTime (2000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.06 * skill->GetLevel () + 0.03 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecskilldamage (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel () + skill->GetT0 () * 5));
            skill->GetVictim ()->SetAmount (skill->GetT0 () == 0 ? 1 : 2);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
#endif
    };
}
#endif
