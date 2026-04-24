#ifndef __CPPGEN_GNET_SKILL4865
#define __CPPGEN_GNET_SKILL4865
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4865:public Skill
    {
      public:
        enum
        { SKILL_ID = 4865 };
          Skill4865 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4865Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar2 (((skill->GetPlayer ()->GetCultivation () == 4) ? skill->GetPlayer ()->GetSkilllevel (5264) : 0));
                skill->GetPlayer ()->
                    SetVar1 ((((skill->GetPlayer ()->GetCultivation () == 4)
                               && (skill->GetPlayer ()->GetSkilllevel (5264) >
                                   0)) ? ((skill->GetPlayer ()->GetRes4 () + skill->GetPlayer ()->GetSkilllevel (5264) * 8) >
                                          600 ? (skill->GetPlayer ()->GetRes4 () +
                                                 skill->GetPlayer ()->GetSkilllevel (5264) * 8) : 600) : (skill->GetPlayer ()->GetLevel () * 3 +
                                                                                                          10 * skill->GetT0 ())));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4865Stub ():SkillStub (4865)
        {
            occupation = 110;
            name = L"¼Æ¡¤Î§Îº¾ÈÕÔ";
            nativename = "¼Æ¡¤Î§Îº¾ÈÕÔ";
            icon = "¼Æ£ºÎ§Îº¾ÈÕÔ.dds";
            maxlevel = 7;
            maxlearn = 6;
            type = 3;
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
            talent[0] = 5197;
            talent_size = 1;
            action[0] = "Ó¢ÕĞ_ÈºÌå³°·í";
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
            action[14] = "Ó¢ÕĞ_ÈºÌå³°·í";
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
            action[29] = "Ó¢ÕĞ_ÈºÌå³°·í";
            action[30] = "0";
            rangetype = 14;
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
        virtual ~ Skill4865Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4862 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (300);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
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
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 90, 94, 98, 102, 106, 110 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel (), 2.5 + 0.5 * skill->GetLevel ());

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
                                                 ((skill->GetPlayer ()->GetType () == 0
                                                   && skill->GetT0 () > 0) ? skill->GetPlayer ()->GetVar1 () : 0));
            skill->GetVictim ()->SetTime (10);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () == 0 ? (-1) : 120));
            skill->GetVictim ()->SetTime (2500 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetAssault2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2500 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (0.01 * skill->GetLevel () * skill->GetPlayer ()->GetMinatk ());
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetPlayer ()->GetVar2 ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecskilldodge (1);
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
