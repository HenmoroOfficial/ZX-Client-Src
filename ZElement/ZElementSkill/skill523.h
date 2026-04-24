#ifndef __CPPGEN_GNET_SKILL523
#define __CPPGEN_GNET_SKILL523
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill523:public Skill
    {
      public:
        enum
        { SKILL_ID = 523 };
          Skill523 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill523Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (556 + 52 * skill->GetLevel () - 2.4 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                     0.55) * (3 * skill->GetLevel () *
                                                                                                                              skill->GetLevel () -
                                                                                                                              7 * skill->GetLevel () +
                                                                                                                              686)) * (1 +
                                                                                                                                       0.07 *
                                                                                                                                       skill->
                                                                                                                                       GetT2 ()));
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->
                    SetPlus ((INT (1.42 - 0.05 * skill->GetLevel ()) *
                              (556 + 52 * skill->GetLevel () - 2.4 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                     0.55) * (3 * skill->GetLevel () *
                                                                                                                              skill->GetLevel () -
                                                                                                                              7 * skill->GetLevel () +
                                                                                                                              686)) * (1 +
                                                                                                                                       0.07 *
                                                                                                                                       skill->
                                                                                                                                       GetT2 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill523Stub ():SkillStub (523)
        {
            occupation = 13;
            name = L"ÓüÁúÆÆ";
            nativename = "ÓüÁúÆÆ";
            icon = "ÓüÁúÆÆ.dds";
            maxlevel = 20;
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
            talent[0] = 631;
            talent[1] = 634;
            talent[2] = 661;
            talent[3] = 665;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_ÓüÁúÆÆ";
            action[1] = "¹íÍõ×Ú_ÓüÁúÆÆ";
            action[2] = "¹íÍõ×Ú_ÓüÁúÆÆ";
            action[3] = "¹íÍõ×Ú_ÓüÁúÆÆ";
            action[4] = "¹íÍõ×Ú_ÓüÁúÆÆ";
            action[5] = "¹íÍõ×Ú_ÓüÁúÆÆ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÓüÁúÆÆ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÓüÁúÆÆ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÓüÁúÆÆ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÓüÁúÆÆ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÓüÁúÆÆ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÓüÁúÆÆ";
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
#endif
        }
        virtual ~ Skill523Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 520 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.7 * skill->GetLevel () * skill->GetLevel () - 5.8 * skill->GetLevel () + 640);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 9000 - 1000 * skill->GetT3 () - 1350 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 115, 117, 119, 121, 123, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.7 * skill->GetLevel () * skill->GetLevel () - 5.8 * skill->GetLevel () + 640),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (556 + 52 * skill->GetLevel () - 2.4 * skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                           0.55) * (3 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () -
                                                                                                                                    7 *
                                                                                                                                    skill->
                                                                                                                                    GetLevel () +
                                                                                                                                    686)), 33,
                               5 * skill->GetLevel () * skill->GetLevel () + 95 * skill->GetLevel (),
                               6 * skill->GetLevel () * skill->GetLevel () + 114 * skill->GetLevel (), 6);

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
            skill->GetVictim ()->SetProbability (1.0 * ((33 + 10 * skill->GetT0 ())));
            skill->GetVictim ()->SetTime (6000 + 100 + 6000 * skill->GetT3 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetPlayer ()->GetMaxhp () + 1);
            skill->GetVictim ()->SetValue ((5 * skill->GetLevel () * skill->GetLevel () + 95 * skill->GetLevel ()) * (1 + 0.6 * skill->GetT0 ()));
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (6000 + 100 + 6000 * skill->GetT3 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.15 * skill->GetPlayer ()->GetMaxmp () + 1);
            skill->GetVictim ()->SetValue ((6 * skill->GetLevel () * skill->GetLevel () + 114 * skill->GetLevel ()) * (1 + 0.6 * skill->GetT0 ()));
            skill->GetVictim ()->SetSubmp (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.12 * skill->GetT1 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            return true;
        }
#endif
    };
}
#endif
