#ifndef __CPPGEN_GNET_SKILL4842
#define __CPPGEN_GNET_SKILL4842
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4842:public Skill
    {
      public:
        enum
        { SKILL_ID = 4842 };
          Skill4842 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4842Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 350;
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
                return 650;
            }
            void Calculate (Skill * skill) const
            {
                float cul = (skill->GetPlayer ()->GetCultivation () == 1 ? skill->GetPlayer ()->GetSkilllevel (5251) * 0.1 : 0);
                int dam =
                    (skill->GetPlayer ()->GetHasbuff (4273) ? skill->GetPlayer ()->GetRage () * 0.01 * skill->GetPlayer ()->GetSkilllevel (4851) : 0);
                  skill->SetPlus (60 + 10 * skill->GetLevel () + dam * cul);
                  skill->SetPlus2 (dam * (1 - cul));
                  skill->SetRatio (1 + (skill->GetPlayer ()->GetCultivation () == 1 ? skill->GetPlayer ()->GetSkilllevel (5252) * 0.05 : 0));
                  skill->SetSkillaccu (skill->GetPlayer ()->
                                       GetHasbuff (4273) ? (10.0 * skill->GetPlayer ()->GetRage () * 100 / skill->GetPlayer ()->GetMaxhp () /
                                                            (30 - 5 * skill->GetPlayer ()->GetSkilllevel (4851))) : 0);
                  skill->GetPlayer ()->
                    SetVar1 ((((skill->GetPlayer ()->GetCultivation () == 1)
                               && (skill->GetPlayer ()->GetSkilllevel (5252) >
                                   0)) ? ((skill->GetPlayer ()->GetRes1 () + skill->GetPlayer ()->GetSkilllevel (5252) * 8) >
                                          600 ? (skill->GetPlayer ()->GetRes1 () +
                                                 skill->GetPlayer ()->GetSkilllevel (5252) * 8) : 600) : (skill->GetPlayer ()->GetLevel () * 3 +
                                                                                                          10 * skill->GetLevel ())));
                  skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4842Stub ():SkillStub (4842)
        {
            occupation = 108;
            name = L"¼ùÌ¤";
            nativename = "¼ùÌ¤";
            icon = "¼ùÌ¤.dds";
            maxlevel = 10;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 13;
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
            talent[0] = 5182;
            talent_size = 1;
            action[0] = "Ó¢ÕĞ_¼ùÌ¤";
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
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "Ó¢ÕĞ_¼ùÌ¤";
            action[15] = "0";
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
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "Ó¢ÕĞ_¼ùÌ¤";
            action[30] = "0";
            rangetype = 14;
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
        virtual ~ Skill4842Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4839 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (10);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
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
            return 12000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 15, 17, 19, 21, 23, 25, 27, 29, 31 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 60 + 10 * skill->GetLevel (), 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () + 10 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (1000);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () == 0 ? (-1) : 120));
            skill->GetVictim ()->SetBlowoff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (4102);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetSkilllevel (5179) >
                                                  0 ? (1 + skill->GetPlayer ()->GetSkilllevel (5179) * 33) : 0));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetCleardebuff (1);
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetSkilllevel (4845) >
                                          0 ? (10000 + 1000 * skill->GetPlayer ()->GetSkilllevel (5181)) : 0);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxhp () * 0.02 * skill->GetPlayer ()->GetSkilllevel (4845));
            skill->GetVictim ()->SetDechurt (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
