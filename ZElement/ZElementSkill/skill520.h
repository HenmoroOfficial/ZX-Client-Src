#ifndef __CPPGEN_GNET_SKILL520
#define __CPPGEN_GNET_SKILL520
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill520:public Skill
    {
      public:
        enum
        { SKILL_ID = 520 };
          Skill520 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill520Stub:public SkillStub
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
                skill->SetRatio ((0.22 + 0.05 * skill->GetT1 ()) * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill520Stub ():SkillStub (520)
        {
            occupation = 13;
            name = L"Ğé¿ÕÕ¶";
            nativename = "Ğé¿ÕÕ¶";
            icon = "Ğé¿ÕÕ¶.dds";
            maxlevel = 4;
            maxlearn = 3;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
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
            talent[0] = 645;
            talent[1] = 667;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[1] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[2] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[3] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[4] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[5] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
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
        virtual ~ Skill520Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 519 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12 + 4 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.46 * skill->GetLevel () * skill->GetLevel () - 1.3 * skill->GetLevel () + 151);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 15000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 106, 114, 122 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10 + 4 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8 + 4 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.46 * skill->GetLevel () * skill->GetLevel () - 1.3 * skill->GetLevel () + 151),
                               22 * skill->GetLevel (),
                               8 + 4 * skill->GetLevel (),
                               5 + 10 * skill->GetLevel (), 20 + 20 * skill->GetLevel (), 8 + 4 * skill->GetLevel (), 90 * skill->GetLevel ());

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
                                                 (norm
                                                  (zrand
                                                   (100 +
                                                    INT (1.6 * (5 + 10 * skill->GetLevel ()) *
                                                         (3 - 2.0 * skill->GetPlayer ()->GetHp () / skill->GetPlayer ()->GetMaxhp ()))) / 100) * 200 +
                                                  (5 + 10 * skill->GetLevel ()) * (3 -
                                                                                   2.0 * skill->GetPlayer ()->GetHp () /
                                                                                   skill->GetPlayer ()->GetMaxhp ())));
            skill->GetVictim ()->SetTime (8000 + 4000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.2 + 0.2 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (90 * skill->GetLevel () * (1 + 0.8 * skill->GetT0 ()) *
                                            (INT (zrand (100) / (100 - 6 * skill->GetT0 ())) + 1));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak5 (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetIscrit () > 0 ? 120 : 0));
            skill->GetVictim ()->SetRatio (11);
            skill->GetVictim ()->SetAmount (11);
            skill->GetVictim ()->SetValue (11);
            skill->GetVictim ()->SetDispel (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (5000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT0 ());
            skill->GetVictim ()->SetInccritrate (1);
            return true;
        }
#endif
    };
}
#endif
